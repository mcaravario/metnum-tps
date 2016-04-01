#!/usr/bin/env python

import pygame, sys
import general
import pygame.mixer
import pygame.mouse
from pygame.locals import KEYDOWN, MOUSEBUTTONDOWN
from field import Field, width, height
from creature import Ball, Player
import os.path

outputTiro = "foo.tiro"
outputJug = "foo.jug"
inputTiro = "foo.tiro"
inputJug = "foo.jug"
arquero = (356, 315, 385, 4)


def addBall(bs, p):
    print "Agregando pelota en ", p
    b = Ball(surface=general.surface, radius=6, showing=True)
    b.set_pos(p)
    bs.append(b)

def addPlayer(ps, p):
    print "Agregado jugador en ", p
    jugador = Player(general.surface, (20, 20), 1, True)
    jugador.set_pos(p)
    ps.append(jugador)

def removeBall(bs):
    if len(bs) == 0:
        print "No hay pelotas"
        return
    b = bs.pop()
    print "Borrando pelota en ", b.get_pos()

def removePlayer(ps):
    if len(ps) == 0:
        print "No hay jugadores"
        return
    p = ps.pop()
    print "Borrando jugador en ", p.get_pos()

def guardar(bs, ps):
    with open(outputTiro, 'w') as f:
        f.write('%d %d %d %d\n' % arquero)
        for b in bs:
            f.write('%d %d\n' % tuple(b.get_pos()))
    with open(outputJug, 'w') as f:
        for p in ps:
            f.write('%d %d\n' % tuple(p.get_pos()))
        

def checkKeys(balls, players):
    for event in pygame.event.get():
        if event.type == KEYDOWN:
            if event.unicode == 'b':
                removeBall(balls)
            elif event.unicode == 'p':
                removePlayer(players)
            elif event.unicode == 'q':
                # Cerrar sin guardar
                pygame.quit()
                sys.exit()
            else:
                guardar(balls, players)
                pygame.quit()
                sys.exit()
        elif event.type == MOUSEBUTTONDOWN:
            if event.button == 1:
                addBall(balls, event.pos)
            else:
                addPlayer(players, event.pos)

def cargarTiros(filename):
    with open(filename,'r') as f:
        # Posicion inicial del arquero. Primera linea del archivo de tiros.
        initpos = map(int, f.readline().strip().split())
        # Posiciones de la pelota.
        s = [map(float, line.strip().split()) for line in f.readlines()]    

    balls = list()
    for pos in s:
        addBall(balls, pos)

    return balls, tuple(initpos)

def cargarJugadores(filename):
    with open(filename,'r') as f:
        posiciones = [map(float, line.strip().split()) 
                for line in f.readlines()]    
    
    players = list()
    for pos in posiciones:
        addPlayer(players, pos)
    return players
    
def main():
    #En argv1 va a estar el archivo de output
    #En argv2 va a estar el archivo de output para los jugadores
    global inputTiro
    global inputJug
    global outputTiro
    global outputJug
    global arquero
    
    argc = len(sys.argv)

    if argc == 5:
        inputTiro = sys.argv[1]
        inputJug = sys.argv[2]
        outputTiro = sys.argv[3]
        outputJug = sys.argv[4]
    elif argc == 3:
        inputTiro = sys.argv[1]
        inputJug = sys.argv[2]
        outputTiro = sys.argv[1]
        outputJug = sys.argv[2]
    else:    
        print 'Modo de uso: python ./main.py archivoInstancia archivoJugadores'
        exit(1)

    print "Archivo de tiro (entrada):", inputTiro
    print "Archivo de Jugadores (entrada):", inputJug
    print "Archivo de tiro (salida):", outputTiro
    print "Archivo de Jugadores (salida):", outputJug


    #Variables
    pygame.init()
    os.environ['SDL_VIDEO_CENTERED'] = '1'
    vis = general.getVisibleSize()
    pygame.display.set_mode( 
            (int(vis[0]), int(vis[1])), pygame.DOUBLEBUF)# , pygame.FULLSCREEN)
    pygame.display.set_caption('TP3 - Instance Builder')
    general.surface = pygame.display.get_surface()
    cameraPos = [0, 0]
    field = Field()
    goal1 = field.getGoal1()

    balls = list()
    players = list()

    # Chequeo si los archivos existen para cargarlos.
    if os.path.isfile(inputTiro):
        balls, arquero = cargarTiros(inputTiro)
    if os.path.isfile(inputJug):
        players = cargarJugadores(inputJug)

    while True:
        goal_size = goal1.image.get_size()
        checkKeys(balls, players)
        field.blitBackground(general.surface)
        field.blitField(width=width, height=height,
                surface=general.surface, cameraPos=cameraPos)

        for player in players:
            player.move(general.surface, field, 0,
                    cameraPos=cameraPos, goal_size=goal_size)
        for ball in balls:
            ball.move(0, cameraPos=cameraPos)
        pygame.display.flip()
        pygame.time.wait(10)

if __name__ == '__main__':
    main()