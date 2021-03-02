#!/usr/bin/env python3

from random import randint, choice
from re import fullmatch

def checkMathId(s):
	if len(s) != 16: return False
	if fullmatch('[0-9]{4}-[0-9]{5}-[0-9]{5}', '1234-12345-12345'): return True
	return False

def genActivationKey():
	s = ""
	for i in range(14):
		s += str(randint(0, 9))
		if i == 3 or i == 7: s += "-"
	return s

def f1(n, b, c):
	for bitIndex in range(8):
		bit = (b >> bitIndex) & 1
		if (bit + ((n - bit) & ~1) == n):
			n = (n - bit) >> 1
		else:
			n = ((c - bit) ^ n) >> 1
	return n

def genPassword(s, h):
	for byteIndex in range(len(s) - 1, -1, -1):
		h = f1(h, ord(s[byteIndex]), 0x105C3)
	n1 = 0
	while f1(f1(h, n1 & 0xFF, 0x105C3), n1 >> 8, 0x105C3) != 0xA5B6:
		n1 += 1
		if (n1 >= 0xFFFF): return "Error"
	n1 = ((n1 + 0x72FA) & 0xFFFF) * 99999 // 0xFFFF
	n1str = ("0000" + str(n1))[-5:]
	temp = int(n1str[0] + n1str[1] + n1str[3] + n1str[4] + n1str[2])
	temp = temp * 0xFFFF // 99999 + 1
	temp = f1(f1(0, temp & 0xFF, 0x1064B), temp >> 8, 0x1064B)
	for byteIndex in range(len(s) - 1, -1, -1):
		temp = f1(temp, ord(s[byteIndex]), 0x1064B)
	n2 = 0
	while f1(f1(temp, n2 & 0xFF, 0x1064B), n2 >> 8, 0x1064B) != 0xA5B6:
		n2 += 1
		if (n2 >= 0xFFFF): return "Error"
	n2 = (n2 & 0xFFFF) * 99999 // 0xFFFF
	n2str = ("0000" + str(n2))[-5:]
	return n2str[3] + n1str[3] + n1str[1] + n1str[0] + "-" + n2str[4] + n1str[2] + n2str[0] + "-" + n2str[2] + n1str[4] + n2str[1] + "::1"

def generate(mathId):
	if not checkMathId(mathId):
		print("This is not a valid MathID !")
	else:
		activationKey = genActivationKey()
		magicNumbers = [10690, 12251, 17649, 24816, 33360, 35944, 36412, 42041, 42635, 44011, 53799, 56181, 58536, 59222, 61041]
		password = genPassword(mathId + "$1&" + activationKey, choice(magicNumbers))
		print("Activation Key:", activationKey)
		print("Password:", password)

mathId = input("Input your MathID (xxxx-xxxxx-xxxxx) and press ENTER: ")
generate(mathId)