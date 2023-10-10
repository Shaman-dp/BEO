import xlrd
import ctypes 
import os
from tkinter import *

try:
	BOOK = xlrd.open_workbook('./Controllers.xls',formatting_info=True)
except:	
	ctypes.windll.user32.MessageBoxW(0, "Не найден файл Controllers.xls", "Ошибка", 0)
	sys.exit()

SHEET = BOOK.sheet_by_index(0)
controllers_list = SHEET.col_values(0,0)

try:
	if controllers_list[0] == '': 1/0 
except:
	ctypes.windll.user32.MessageBoxW(0, "Пустой файл", "Ошибка", 0)
	sys.exit()

root = Tk()
root.geometry('400x730')
root.title('Настройки')

ustavki = [ '5000', '220', '1220', '10000']
AIS_brk_isEO = list(range(len(controllers_list)))
AIS_brk_isAO = list(range(len(controllers_list)))
AIS_brk_isVO = list(range(len(controllers_list)))
count_ustavki = 0
count_controllers = 0

ustavkiFrame = Frame(root)
controllersFrame = Frame(root)

def menu_ustavki():

	controllersFrame.pack_forget()
	ustavkiFrame.pack(side=LEFT)

	global count_ustavki

	def get_data():
		if pulse_t_data.get() != '': ustavki[0] = pulse_t_data.get()
		else:
			ctypes.windll.user32.MessageBoxW(0, "Не задана периодичность выдачи пульса в БЭО", "Ошибка", 0) 
		if pulse_link_t_data.get() != '': ustavki[1] = pulse_link_t_data.get()
		else:
			ctypes.windll.user32.MessageBoxW(0, "Не задана периодичность выдачи пульса по сети", "Ошибка", 0)
		if noLink_sp_data.get() != '': ustavki[2] = noLink_sp_data.get()
		else:
			ctypes.windll.user32.MessageBoxW(0, "Не задано время отсутствия связи с АИС", "Ошибка", 0)
		if start_delay_data.get() != '': ustavki[3] = start_delay_data.get()	
		else:
			ctypes.windll.user32.MessageBoxW(0, "Не задана задержка включения БЭО при запуске АИС", "Ошибка", 0)
		if (pulse_t_data.get() != '') & (pulse_link_t_data.get() != '') & (noLink_sp_data.get() != '') & (start_delay_data.get() != ''): root.destroy()


	if count_ustavki == 0:
		count_ustavki+=1	
		pulse_t = Label(ustavkiFrame, text="Периодичность выдачи пульса в БЭО, мс	").grid(column=0, row=0) 
		pulse_link_t = Label(ustavkiFrame, text="Периодичность выдачи пульса по сети, мс	").grid(column=0, row=1) 
		noLink_sp = Label(ustavkiFrame, text="Время отсутствия связи с АИС, мс		").grid(column=0, row=2) 
		start_delay = Label(ustavkiFrame, text="       Задержка включения БЭО при запуске АИС, мс").grid(column=0, row=3) 

		pulse_t_data = Entry(ustavkiFrame, width=6)
		pulse_t_data.insert(0, "5000")
		pulse_t_data.grid(column=1, row=0, padx=30)
		pulse_link_t_data = Entry(ustavkiFrame, width=6)
		pulse_link_t_data.insert(0, "220")
		pulse_link_t_data.grid(column=1, row=1, padx=30)
		noLink_sp_data = Entry(ustavkiFrame, width=6)
		noLink_sp_data.insert(0, "1220")
		noLink_sp_data.grid(column=1, row=2, padx=30)
		start_delay_data = Entry(ustavkiFrame, width=6)
		start_delay_data.insert(0, "10000")
		start_delay_data.grid(column=1, row=3, padx=30)

		space = Label(ustavkiFrame, text="").grid(column=2, row=3)
		btn = Button(ustavkiFrame, text="Записать данные", width=50, height=2, command=get_data).grid(column=0, row=4, columnspan = 2, pady=10)

def menu_controllers(): 

	ustavkiFrame.pack_forget()
	controllersFrame.pack(side=LEFT)

	global count_controllers

	if count_controllers == 0:

		count_controllers+=1
		name_AIS = Label(controllersFrame, text='   Контроллер	').grid(column=0, row=0)
		AIS_isEO = Label(controllersFrame, text='ЭО ').grid(column=1, row=0)
		AIS_isAO = Label(controllersFrame, text='АО ').grid(column=2, row=0)
		AIS_isVO = Label(controllersFrame, text='ВО ').grid(column=3, row=0)

		for controller in range(len(controllers_list)):
			controller_name = '   ' + str(controllers_list[controller]).replace(' ', '')
			AIS = Label(controllersFrame, text=controller_name).grid(column=0, row=controller+1, sticky=W)
			AIS_brk_isEO[controller] = IntVar()
			Checkbutton(controllersFrame, variable=AIS_brk_isEO[controller]).grid(column=1, row=controller+1)
			AIS_brk_isAO[controller] = IntVar()
			Checkbutton(controllersFrame, variable=AIS_brk_isAO[controller]).grid(column=2, row=controller+1)
			AIS_brk_isVO[controller] = IntVar()
			Checkbutton(controllersFrame, variable=AIS_brk_isVO[controller]).grid(column=3, row=controller+1)

def menu_help(): os.startfile("ALG_BEO_Template_Инструкция.docx")

def menu_exit(): root.destroy()

main_menu = Menu(root)
root.config(menu=main_menu)

main_menu.add_cascade(label="Уставки", command=menu_ustavki)
main_menu.add_cascade(label="Участие в остановах", command=menu_controllers)
main_menu.add_cascade(label="Помощь", command=menu_help)
main_menu.add_cascade(label="Выход", command=menu_exit)

menu_controllers()

root.mainloop()

Template = open('./ALG_BEO.cpp', 'w')

Template.write('#include "ais.h"\n#include "AISPar.h"\n#include "externs.h"\n#include "UDT_system.h"\n#include "TLS_COM.H"\n\n')
Template.write('unsigned short int BEO_timer;                           //счётчик пульсов для включения БЭО в работу при запуске контроллера\n')
Template.write('unsigned short int BEO_Pulse;                           //счётчик циклов для выдачи пульса в БЭО\n')
Template.write('unsigned short int BEO_Pulse_Link;                      //счётчик пульсов для включения БЭО в работу при запуске контроллера\n\n')

Template.write('//Счётчики для диагностики связи с АИС по сети\n')
for controller in range(len(controllers_list)):
	controller_name = str(controllers_list[controller]).replace(' ', '')
	Template.write('unsigned short int ' + 'BEO_Delay_' + controller_name + '_v, BEO_Delay_' + controller_name + '_n;\n')

Template.write('\n//состояние исправности контроллера\n')
for controller in range(len(controllers_list)):
	controller_name = str(controllers_list[controller]).replace(' ', '')
	Template.write('bool ' + controller_name + '_ctrl_BEO = true;\n')

Template.write('\nvoid LinkBEO(void)\n{\n//уставки\n')
Template.write('unsigned short int BEO_pulse_t      = (unsigned short int)('+ ustavki[0] + '/cycleTime.base);  //периодичность выдачи пульса в БЭО                                 //раз в 1 сек\n')
Template.write('unsigned short int BEO_pulse_link_t = (unsigned short int)('+ ustavki[1] + '/cycleTime.base);   //периодичность выдачи пульса по сети                               //раз в 220 мс\n')
Template.write('unsigned short int BEO_noLink_sp    = (unsigned short int)('+ ustavki[2] + '/cycleTime.base);  //Уставка по времени отсутствия связи с другими АИС для запуска ЭО  //нет связи в течение 1,2 сек\n')
Template.write('unsigned short int BEO_start_delay  = (unsigned short int)('+ ustavki[3] + '/cycleTime.base); //Задержка включения БЭО в работу при запуске контроллера           //10 сек\n\n')

Template.write('//Таймер БЭО\nBEO_timer++;\nif (BEO_timer > BEO_start_delay) BEO_timer = BEO_start_delay;   //Задержка на запуск БЭО\nif (BEO_timer >= BEO_start_delay - 1){\ndgo.WatchDog_ON=true; //Включить БЭО в работу\n\nBEO_Pulse++;          //сигнал на физический контроллер\nBEO_Pulse_Link++;    //сигнал по сети\n\nif (BEO_Pulse_Link >= BEO_pulse_link_t) {	alg.WatchDog_Pulse_UCP1 = !alg.WatchDog_Pulse_UCP1;\n											BEO_Pulse_Link = 0;\n}\n')

Template.write('/*///////////////////////////////////////////////////////////////////////\n                      Анализ работы АИСов\n//////////////////////////////////////////////////////////////////////*/ \n\n')
for controller in range(len(controllers_list)):
	controller_name = str(controllers_list[controller]).replace(' ', '')
	Template.write('//' + controller_name + '\n')
	Template.write('if (' + controller_name + '.alg_WatchDog_Pulse_' + controller_name + ') {   BEO_Delay_' + controller_name + '_v++;\n																BEO_Delay_' + controller_name + '_n = 0;}\n')
	Template.write('else									' + '{   BEO_Delay_' + controller_name + '_n++;\n																BEO_Delay_' + controller_name + '_v = 0;}\n\n')
	Template.write('anpar.' + controller_name + '_noLink_time = cycleTime.base*maxValue( TNANFloat(BEO_Delay_' + controller_name + '_v), TNANFloat(BEO_Delay_' + controller_name + '_n) );\n')
	Template.write('alg.' + controller_name + '_brk = ((BEO_Delay_' + controller_name + '_v >= BEO_noLink_sp) || (BEO_Delay_' + controller_name + '_n >= BEO_noLink_sp)) && ! tch.imit;\n')
	Template.write('if (!alg.' + controller_name + '_brk) ' + controller_name + '_ctrl_BEO = false;\n\n')

Template.write('alg.AIS_brk_EO = 	//Обобщённый признак обрыва связи с учётом участия АИС в запуске ЭО\n')
for controller in range(len(controllers_list)):
	controller_name = str(controllers_list[controller]).replace(' ', '')
	if controller == len(controllers_list) - 1: Template.write('					alg.' + controller_name + '_brk && alg.' + controller_name + '_brk_isEO;\n')
	else: Template.write('					alg.' + controller_name + '_brk && alg.' + controller_name + '_brk_isEO ||\n')

Template.write('//блокировка подачи импульсов в БЭО при неисправности\n')
Template.write('if (!alg.AIS_brk_EO) {dgo.WatchDog_Pulse = !dgo.WatchDog_Pulse; //Выдаём пульс в БЭО, когда связь с АИС исправна\n\n')

Template.write('alg.AIS_brk_AO = 	//Обобщённый признак обрыва связи с учётом участия АИС в запуске АО\n')
for controller in range(len(controllers_list)):
	controller_name = str(controllers_list[controller]).replace(' ', '')
	if controller == len(controllers_list) - 1: Template.write('					alg.' + controller_name + '_brk && alg.' + controller_name + '_brk_isAO || smd.PZ;\n')
	else: Template.write('					alg.' + controller_name + '_brk && alg.' + controller_name + '_brk_isAO ||\n')

Template.write('alg.AIS_brk_VO = 	//Обобщённый признак обрыва связи с учётом участия АИС в запуске ВО\n')
for controller in range(len(controllers_list)):
	controller_name = str(controllers_list[controller]).replace(' ', '')
	if controller == len(controllers_list) - 1: Template.write('					alg.' + controller_name + '_brk && alg.' + controller_name + '_brk_isVO || smd.PZ;\n}\n\n')
	else: Template.write('					alg.' + controller_name + '_brk && alg.' + controller_name + '_brk_isVO ||\n')

Template.write('//сброс счетчиков\nelse\n{\n')
Template.write('	BEO_Pulse = BEO_Pulse_Link = 0;\n')
for controller in range(len(controllers_list)):
	controller_name = str(controllers_list[controller]).replace(' ', '')
	Template.write('	BEO_Delay_' + controller_name + '_v = BEO_Delay_' + controller_name + '_n = 0;\n')

Template.write('}\n}')

BOOK.release_resources()
Template.close()