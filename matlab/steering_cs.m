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

start = 'o';
fwrite(igotit, start, 'int32')
for i = 1:count
tline = fread(igotit, [2000,1], 'int32');
array = [array; tline];
end

start = 'z'; %left 29
fwrite(igotit, start, 'int32');
for i = 1:count
tline = fread(igotit, [2000,1], 'int32');
array = [array; tline];
end

start = 'x'; %right -29
fwrite(igotit, start, 'int32');
for i = 1:count
tline = fread(igotit, [2000,1], 'int32');
array = [array; tline];
end

fclose(igotit);

plot(array)
grid on;
hold on;

disp 'Done!'

%delete(igotit);
%clear igotit
