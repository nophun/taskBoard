def calc_crc(text):
	crc = 0
	for c in text:
		crc ^= c
	return crc

def clean_text(text):
	char_map = [
		['å', 'a'],
		['ä', 'a'],
		['ö', 'o'],
		['Å', 'A'],
		['Ä', 'A'],
		['Ö', 'O']
	]
	text = text[:200]
	for pair in char_map:
		text = text.replace(pair[0], pair[1])

	return text


# title = "............................"
title = "jmm..a.mmm123"
# title = "Indigö updäte"
desc = "Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type a"
title = clean_text(title)
desc = clean_text(desc)
bts = [0]
bts += len(title).to_bytes(1)
bts += bytes(title, 'utf-8')
bts += len(desc).to_bytes(1)
bts += bytes(desc, 'utf-8')
crc = calc_crc(bts)
bts += crc.to_bytes(1)

import serial.tools.list_ports
import serial
# ports = serial.tools.list_ports.comports()
# for port, desc, hwid in sorted(ports):
#     print("{}: {}".format(port, desc))

ser = serial.Serial(port="COM54", baudrate=115200, bytesize=8, parity='N', stopbits=1, timeout=2)
ser.write(bts)
print(bytes(bts))
recv_buf = ""
while True:
	x = ser.read()
	if len(x) == 0:
		break
	else:
		recv_buf += x.decode('utf-8')
		# print(x)

print(recv_buf)
print("done")

