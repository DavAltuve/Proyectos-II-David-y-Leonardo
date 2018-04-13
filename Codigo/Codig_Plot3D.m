ID=fopen('Datos.txt','r');
A=textscan(ID,'%f %f %f');
A=cell2mat(A);
figure(1)
plot3(A(:,1),A(:,2),A(:,3),':.b');
grid();
zlim([0 10]);
figure(2)
plot(A(:,1),A(:,2),':.b');
fclose(ID);
