global dat
delete(instrfind);
dat = serial('COM11', 'BaudRate', 115200);
dat.InputBufferSize = 4096;

fopen(dat)
set(dat, 'ByteOrder', 'littleEndian')

disp 'Connection - done!'

start = 'a'; % let's start 
fwrite(dat, start);

temp_data=[];
all_data = [];
% get stop-data
t = 1;
 for i = 1:t
    temp_data=fread(dat, [1000,1], 'int16');
    all_data = [all_data; temp_data];
 end
 
start = 'w'; % go forward
fwrite(dat, start);

t = 2;
 for i = 1:t
    temp_data=fread(dat, [1000,1], 'int16');
    all_data = [all_data; temp_data];
 end
 start = ' '; % stop
fwrite(dat, start);

t = 2;
 for i = 1:t
    temp_data=fread(dat, [1000,1], 'int16');
    all_data = [all_data; temp_data];
 end

  
fclose(dat);
plot(all_data)
disp 'Close connection'