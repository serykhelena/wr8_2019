global dat
delete(instrfind);
dat = serial('COM15', 'BaudRate', 115200);
dat.InputBufferSize = 4096;

fopen(dat)
set(dat, 'ByteOrder', 'littleEndian')
disp 'Ok!'

fwrite(dat,'q','char');

 A = [];
 B = [];

fwrite(dat,'w','char'); %go forward
for i = 1:10
    A=fread(dat, [100,1], 'int16');
    B = [B; A];
end
 
fwrite(dat,'s','char');
for i = 1:10
    A=fread(dat, [100,1], 'int16');
    B = [B; A];
end

fwrite(dat,' ','char');

fclose(dat);

LPF = [];
NO_LPF = [];

for i = 1:2: length(B) - 1
    
    NO_LPF = [NO_LPF, B(i)];
    LPF = [LPF, B(i + 1)];
    
end 

plot(LPF)
hold on
plot(NO_LPF)
disp 'Close'