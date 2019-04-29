global dat
delete(instrfind);
dat = serial('COM7', 'BaudRate', 115200);
dat.InputBufferSize = 4096;

fopen(dat)
set(dat, 'ByteOrder', 'littleEndian')

disp 'Ok!'

A=[];
B = [];
t = 1000;
 for i = 1:t
    A=fread(dat, [1,1], 'int16');
    B = [B; A];
 end
fclose(dat);
plot(B)
disp 'Close'