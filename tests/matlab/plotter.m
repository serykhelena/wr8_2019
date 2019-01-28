global dat
delete(instrfind);
dat = serial('COM8', 'BaudRate', 115200);
dat.InputBufferSize = 4096;

fopen(dat)
set(dat, 'ByteOrder', 'littleEndian')

disp 'Ok!'

A=[];
B = [];
t = 10;
 for i = 1:t
%     A = fread(dat, 1, 'uint16');
    A=fread(dat, [100,1], 'int16');
    %A1 = A;
    B = [B; A];
 end
fclose(dat);
plot(B)
disp 'Close'