import pygame, sys
import general
import random
import pygame.mixer
from pygame.locals import *
from field import *
from creature import *
import math
import os.path
import numpy
from scipy import interpolate

umbralAtajada = 7
delta= 18
rango= 0

def pressAnyKeyToExit():
	for event in pygame.event.get():
		if event.type == KEYDOWN:
			pygame.quit()
			sys.exit()

def moverArquero(t):
	posicionArquero = arquero.get_pos()	
	posicionArquero[1] += movimientosArquero[t]

	arquero.set_pos(posicionArquero)
	arquero.move(general.surface, field, 0, cameraPos=cameraPos, goal_size=goal_size)	
	line_color = pygame.Color(240,250,220)
	drawCircle(general.surface, line_color, arquero.getCentre(), umbralAtajada)
	
def festejo():
	ultimo10= pygame.image.load("./images/roman.jpg").convert()
	ultimo10= pygame.transform.scale(ultimo10,(500,300))
	transparencia= 0
	gritoDeGol = pygame.mixer.Sound("./sounds/GolRoman.wav")
	channel = gritoDeGol.play()	
	while True:
		pressAnyKeyToExit()
		field.blitBackground(general.surface)
		field.blitField(width=width, height=height, surface=general.surface, cameraPos=cameraPos)
		arquero.move(general.surface, field, 0, cameraPos=cameraPos, goal_size=goal_size)
		pelotaPos = pelota.get_pos();
		if pelotaPos[0]>arquero.get_pos()[0]-7:
			pelotaPos[0] -= 1
			pelota.set_pos(pelotaPos)
		pelota.move(0, cameraPos=cameraPos)
		for player in players:
			if player.get_pos()[0]>80 or player.get_pos()[1]>80:
				player.fireLeft()
				player.fireUp()
			elif player.get_pos()[0]<30:
				player.fireRight()
			elif player.get_pos()[1]<30:
				player.fireDown()
			else:
				player.stopLeftAndRight()
				player.stopUpAndDown()
			player.collidingGoal(goal1)
			player.move(general.surface, field, 0, cameraPos=cameraPos, goal_size=goal_size)
		ultimo10.set_alpha(transparencia)
		transparencia= transparencia+1
		general.surface.blit(ultimo10,[600,300])
		pygame.display.flip()
		pygame.time.wait(10)

def leerPosicionesPelota():
	global rango
	f = open(sys.argv[2],'r')
	# Posicion inicial del arquero. Primera linea del archivo de tiros.
	initpos = map(int,f.readline().strip().split())
	# arquero.set_pos([initpos[0]-10,initpos[1]-10])
	arquero.set_pos([125 - 10,initpos[0]-10])
	rango= initpos[3]
	# Posiciones de la pelota.
	s = [map(float,line.strip().split()) for line in f.readlines()]	
	return s

def leerPosicionesJugadores():
	f = open(sys.argv[3],'r')
	posiciones = [map(float,line.strip().split()) for line in f.readlines()]	
	for i in range(len(posiciones)):
		posiciones[i]= [posiciones[i][0]-10,posiciones[i][1]-10]
	return posiciones
	
def leerMovimientosArquero():
	f = open(sys.argv[1],'r')
	s = map(float,[w.strip() for w in f.readlines()])
	for mov in s:
		if math.fabs(mov) > rango:
			print "El movimiento del arquero no es valido."
			print "mov: ", mov
			print "rango: ", rango
			exit(1)
	return s

def suavizar(posicionesPelota,movimientosArquero):	
	npuntos= len(posicionesPelota)
	resPosPelota= []
	resMovsArquero= []

	for i in xrange(npuntos-1):
		t1= posicionesPelota[i]
		t2= posicionesPelota[i+1]
		deltax= (t2[0] - t1[0]) / delta
		deltay= (t2[1] - t1[1]) / delta
		deltaArquero= (movimientosArquero[i]) / delta
		for k in xrange(delta):
			resPosPelota.append([t1[0]+(deltax*k),t1[1]+(deltay*k)])			
			resMovsArquero.append(deltaArquero)
		# for k in xrange(movimientosArquero[i] % delta):
		# 	resMovsArquero[(i*delta)+k]+=1
	return resPosPelota, resMovsArquero
	
if __name__ == '__main__':
	#En argv1 va a estar el arquero
	#En argv2 va a estar el tiro	
	
	if len(sys.argv)!=4:
		print 'Modo de uso: python ./main.py archivoArquero archivoTiro archivoJugadores'
		exit(1)
	
	#Variables
	pygame.init()
	os.environ['SDL_VIDEO_CENTERED'] = '1'
	vis = general.getVisibleSize()
	window = pygame.display.set_mode( (int(vis[0]), int(vis[1])), pygame.DOUBLEBUF)# , pygame.FULLSCREEN)
	pygame.display.set_caption('TP3')
	general.surface = pygame.display.get_surface()
	cameraPos = [0, 0]
	field = Field();
	goal1 = field.getGoal1()

	# gritoDeGol = pygame.mixer.Sound("./sounds/dePaoli.wav")
	# gritoDeGol.play()
	
	pelota = Ball(surface=general.surface, radius=6, showing=True)
	arquero = Player(general.surface, (20, 20), 2, True)

	posiciones = leerPosicionesJugadores()
	posicionesPelota = leerPosicionesPelota()
	movimientosArquero = leerMovimientosArquero()
	posicionesPelota, movimientosArquero= suavizar(posicionesPelota,movimientosArquero)
	
	number_of_players = len(posiciones)
	players = list()
	for i in xrange(0, number_of_players):
		player = Player(general.surface, (20, 20), 1, True)
		player.set_pos(posiciones[i])
		players.append(player)

	for i in xrange(0, len(posicionesPelota)):
		#For principal de la animacion
		goal_size = goal1.image.get_size()

		#Campo de juego
		field.blitBackground(general.surface)
		field.blitField(width=width, height=height, surface=general.surface, cameraPos=cameraPos)

		#Arquero
		moverArquero(i)

		# Jugadores de campo
		for player in players:
			player.move(general.surface, field, 0, cameraPos=cameraPos, goal_size=goal_size)

		#Pelota
		pelota.set_pos(posicionesPelota[i])
		arqueroX, arqueroY= arquero.getCentre()[0], arquero.getCentre()[1]
		pelotaX, pelotaY= pelota.getCentre()[0], pelota.getCentre()[1]
		# if pelota.get_pos()[1] in range(arquero.get_pos()[1]-umbralAtajada, arquero.get_pos()[1]+umbralAtajada):
		f = open('./resultados/veredictos.out','a')
		arcoX= field.getGoal1().rect.left + field.getGoal1().image.get_size()[0]

		if ((pelotaX - arqueroX)**2 + (pelotaY - arqueroY)**2) <= umbralAtajada**2:
			# Ataja
			f.write(sys.argv[1]+' '+sys.argv[2]+' '+"\n")
			f.close()
			gritoDeGol = pygame.mixer.Sound("./sounds/PatoPosta.wav")
			gritoDeGol.play()
			ultimo1= pygame.image.load("./images/chila.jpg").convert()
			ultimo1= pygame.transform.scale(ultimo1,(500,300))
			transparencia= 0
			while True:
				ultimo1.set_alpha(transparencia)
				transparencia= transparencia+1
				general.surface.blit(ultimo1,[600,300])
				pygame.display.flip()
				pygame.time.wait(10)
				pressAnyKeyToExit()
		elif pelota.get_pos()[0] < arcoX - 14:
			# Golazo
			f.write(sys.argv[1]+' '+sys.argv[2]+' '+"0\n")
			f.close()
			festejo()
		
		pelota.rot_center(1)
		pelota.move(0, cameraPos=cameraPos)
		#Muestra todo
		pygame.display.flip()
		pygame.time.wait(10)