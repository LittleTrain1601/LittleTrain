# -*- coding: utf-8 -*-
f = open('track.txt', 'w');
i=0;
j=0;
while i<24:
	j = 0
	while j<80:
		f.write('.')
		j += 1
	f.write('\n')
	i += 1
f.close()
