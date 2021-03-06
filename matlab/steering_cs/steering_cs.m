instrreset
global igotit

igotit = serial('COM4','BaudRate',115200);
igotit.InputBufferSize = 4096;

fopen(igotit);
set(igotit,'ByteOrder','littleEndian');

disp 'Ok!'

tline = [];
array = [];
count = 2;
ref = [];
real = [];

start = 'o';
fwrite(igotit, start, 'int16')
for i = 1:count
    tline = fread(igotit, [1000,1], 'int16');
    array = [array; tline];
end

start = 'z'; %left 29
fwrite(igotit, start, 'int16');
for i = 1:count
    tline = fread(igotit, [1000,1], 'int16');
    array = [array; tline];
end

start = 'x'; %right -29
fwrite(igotit, start, 'int16');
for i = 1:count
    tline = fread(igotit, [1000,1], 'int16');
    array = [array; tline];
end

fclose(igotit);

for i = 1:2:(length(array)-1)
    real = [real; array(i,1)];
end

for j = 2:2:(length(array))
    ref = [ref; array(j,1)];
end

plot(real,'r')
grid on;
hold on;
plot(ref)

disp 'Done!'

delete(igotit);
clear igotit
