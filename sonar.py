import time

import pygame
import serial
import os
import math
import sys
from time import sleep

RED = (255, 0, 0)
GREEN = (0, 255, 0)
DIM_GREEN = (0, 128, 0)
GRAY = (192, 192, 192)
DIM_GRAY = (128, 128, 128)

distance_sonic = 0
if __name__ == '__main__':
    def draw_sonar(pos, distance_sonic, distance_infra):
        gameDisplay.fill((0, 0, 0))
        angle_text = font.render('Angle: ' + str(pos), False, GREEN)
        sonic_text = font.render('Distance to sonic: ' + str(distance_sonic), False, DIM_GREEN)
        infrared_text = font.render('Distance to infrared: ' + str(int(distance_infra)), False, DIM_GRAY)
        gameDisplay.blit(angle_text, (80, 450))
        gameDisplay.blit(sonic_text, (80, 500))
        gameDisplay.blit(infrared_text, (80, 550))
        pygame.draw.rect(gameDisplay, GREEN, pygame.Rect(30, 30, 730, 400), border_radius=2, width=4)
        pygame.draw.rect(gameDisplay, GRAY, pygame.Rect(780, 30, 500, 200), border_radius=1, width=4)
        for i in range(0, 180):
            pygame.draw.line(gameDisplay, color=DIM_GREEN, start_pos=(36+i*4, 430), end_pos=(36+i*4, 430 - list_of_green_sonic[i]*2), width=4)
        for i in range(0, 180):
            pygame.draw.line(gameDisplay, color=DIM_GRAY, start_pos=(780+i*2.8, 230), end_pos=(780+i*2.8, 230 - list_of_green_infrared[i]*2), width=3)

    try:
        serial_conn = serial.Serial(port='COM5', baudrate=9600)
    except serial.SerialException:
        print('No device found on COM5.\nExiting...')
        sys.exit(1)

    pygame.init()
    size = (1360, 720)
    gameDisplay = pygame.display.set_mode(size)
    pygame.display.set_caption("Sonar")
    sysfont = pygame.font.get_default_font()
    font = pygame.font.SysFont(None, 48)


    list_of_green_sonic = []
    list_of_green_infrared = []

    for i in range(0,180):
            list_of_green_sonic.append(200)
            list_of_green_infrared.append(100)
    while True:
        sleep(0.005)
        rawdata = serial_conn.readline()

        old_distance_sonic = distance_sonic
        distance_sonic = int(rawdata.decode('utf-8').split(' ')[0])
        if distance_sonic == 0:
            distance_sonic = old_distance_sonic
        distance_infra = float(rawdata.decode('utf-8').split(' ')[1])
        pos = int(rawdata.decode('utf-8').split(' ')[2])
        print(distance_sonic, distance_infra, pos)
        if distance_sonic >= 200:
            distance_sonic = 200
        if distance_infra >= 100:
            distance_infra = 100
        list_of_green_sonic[pos] = distance_sonic
        list_of_green_infrared[pos] = distance_infra


        draw_sonar(pos, distance_sonic, distance_infra)
        pygame.display.update()