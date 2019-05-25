global dat
delete(instrfind);
dat = serial('COM11', 'BaudRate', 115200);
dat.InputBufferSize = 4096;

fopen(dat)
set(dat, 'ByteOrder', 'littleEndian')
disp 'Ok!'

fwrite(dat,'a','uint8');

 A = [];
 B = [];

fwrite(dat,'z', 'uint8'); %go forward
for i = 1:10
    A=fread(dat, [100,1], 'int16');
    B = [B; A];
end
 
fwrite(dat,'s', 'uint8');
for i = 1:10
    A=fread(dat, [100,1], 'int16');
    B = [B; A];
end

fwrite(dat,' ', 'uint8');
fwrite(dat,'x', 'uint8');
fclose(dat);

LPF = [];
NO_LPF = [];

for i = 1:2: length(B) - 1
    
    NO_LPF = [NO_LPF, B(i)];
    LPF = [LPF, B(i + 1)];
    
end 

plot(LPF)
hold on
grid on
plot(NO_LPF)
disp 'Close'