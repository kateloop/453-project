import socket
import time
import winsound


def play(freq):
     if freq == "0":
        winsound.PlaySound("Off.wav", winsound.SND_ASYNC | winsound.SND_FILENAME)
     elif freq == "131":
        winsound.PlaySound("C3.wav", winsound.SND_ASYNC | winsound.SND_FILENAME)
     elif freq == "139":
        winsound.PlaySound("C3S.wav", winsound.SND_ASYNC | winsound.SND_FILENAME)
     elif freq == "147":
        winsound.PlaySound("D3.wav", winsound.SND_ASYNC | winsound.SND_FILENAME)
     elif freq == "156":
        winsound.PlaySound("D3S.wav", winsound.SND_ASYNC | winsound.SND_FILENAME)
     elif freq == "165":
        winsound.PlaySound("E3.wav", winsound.SND_ASYNC | winsound.SND_FILENAME)
     elif freq == "175":
        winsound.PlaySound("F3.wav", winsound.SND_ASYNC | winsound.SND_FILENAME)
     elif freq == "185":
        winsound.PlaySound("F3S.wav", winsound.SND_ASYNC | winsound.SND_FILENAME)
     elif freq == "196":
        winsound.PlaySound("G3.wav", winsound.SND_ASYNC | winsound.SND_FILENAME)
     elif freq == "208":
        winsound.PlaySound("G3S.wav", winsound.SND_ASYNC | winsound.SND_FILENAME)
     elif freq == "220":
        winsound.PlaySound("A3.wav", winsound.SND_ASYNC | winsound.SND_FILENAME)
     elif freq == "233":
        winsound.PlaySound("A3S.wav", winsound.SND_ASYNC | winsound.SND_FILENAME)
     elif freq == "247":
        winsound.PlaySound("B3.wav", winsound.SND_ASYNC | winsound.SND_FILENAME)
     elif freq == "262":
        winsound.PlaySound("C4.wav", winsound.SND_ASYNC | winsound.SND_FILENAME)
     elif freq == "277":
        winsound.PlaySound("C4S.wav", winsound.SND_ASYNC | winsound.SND_FILENAME)
     elif freq == "294":
        winsound.PlaySound("D4.wav", winsound.SND_ASYNC | winsound.SND_FILENAME)
     elif freq == "311":
        winsound.PlaySound("D4S.wav", winsound.SND_ASYNC | winsound.SND_FILENAME)
     elif freq == "330":
        winsound.PlaySound("E4.wav", winsound.SND_ASYNC | winsound.SND_FILENAME)
     elif freq == "349":
        winsound.PlaySound("F4.wav", winsound.SND_ASYNC | winsound.SND_FILENAME)
     elif freq == "370":
        winsound.PlaySound("F4S.wav", winsound.SND_ASYNC | winsound.SND_FILENAME)
     elif freq == "392":
        winsound.PlaySound("G4.wav", winsound.SND_ASYNC | winsound.SND_FILENAME)
     elif freq == "415":
        winsound.PlaySound("G4S.wav", winsound.SND_ASYNC | winsound.SND_FILENAME)
     elif freq == "440":
        winsound.PlaySound("A4.wav", winsound.SND_ASYNC | winsound.SND_FILENAME)
     elif freq == "466":
        winsound.PlaySound("A4S.wav", winsound.SND_ASYNC | winsound.SND_FILENAME)
     elif freq == "494":
        winsound.PlaySound("B4.wav", winsound.SND_ASYNC | winsound.SND_FILENAME)
     elif freq == "523":
        winsound.PlaySound("C5.wav", winsound.SND_ASYNC | winsound.SND_FILENAME)
     elif freq == "554":
        winsound.PlaySound("C5S.wav", winsound.SND_ASYNC | winsound.SND_FILENAME)
     elif freq == "587":
        winsound.PlaySound("D5.wav", winsound.SND_ASYNC | winsound.SND_FILENAME)
     elif freq == "622":
        winsound.PlaySound("D5S.wav", winsound.SND_ASYNC | winsound.SND_FILENAME)
     elif freq == "659":
        winsound.PlaySound("E5.wav", winsound.SND_ASYNC | winsound.SND_FILENAME)
     elif freq == "698":
        winsound.PlaySound("F5.wav", winsound.SND_ASYNC | winsound.SND_FILENAME)
     elif freq == "740":
        winsound.PlaySound("F5S.wav", winsound.SND_ASYNC | winsound.SND_FILENAME)
     elif freq == "784":
        winsound.PlaySound("G5.wav", winsound.SND_ASYNC | winsound.SND_FILENAME)
     elif freq == "831":
        winsound.PlaySound("G5S.wav", winsound.SND_ASYNC | winsound.SND_FILENAME)
     elif freq == "880":
        winsound.PlaySound("A5.wav", winsound.SND_ASYNC | winsound.SND_FILENAME)
     elif freq == "932":
        winsound.PlaySound("A5S.wav", winsound.SND_ASYNC | winsound.SND_FILENAME)
     elif freq == "988":
        winsound.PlaySound("B5.wav", winsound.SND_ASYNC | winsound.SND_FILENAME)
     
             
             


UDP_IP="128.104.180.233"
UDP_PORT=1222

sock  = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((UDP_IP, UDP_PORT))

freqVals = [131, 139, 147, 156, 165, 175, 185, 196, 208, 220, 233, 247, 262, 277, 294, 311, 330, 349, 378, 392, 415, 440, 466, 494, 523, 554, 587, 622, 659, 698, 740, 784, 831, 880, 932, 988]


while True:
    note, addr = sock.recvfrom(1024) # buffer size of 1024 bytes
    print "",note[0]+note[1]+note[2],""
    freq = note[0] + note[1] + note[2]
    play(freq)

