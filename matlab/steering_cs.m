instrreset

igotit = serial('COM4');
set(igotit,'BaudRate',115200);
igotit.InputBufferSize = 4096;

set(igotit,'ByteOrder','littleEndian');
fopen(igotit);

start = 'o';
fwrite(igotit, start, 'int32')

tline = [];
array = [];
count = 2;

for i=1:count
tline= fread(igotit,[2000,1],'int32');
array = [array; tline];
end

start = 'z'; %left 29
fwrite(igotit, start, 'int32');

for i=1:count
tline= fread(igotit,[2000,1],'int32');
array = [array; tline];
end

start = 'x'; %right -29
fwrite(igotit, start, 'int32');

for i=1:count
tline= fread(igotit,[2000,1],'int32');
array = [array; tline];
end

fclose(igotit);

plot(array)
grid on;
hold on;

%delete(igotit);
%clear igotit
