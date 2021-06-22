MovWriter = VideoWriter('../results/simulation.mp4','MPEG-4');
MovWriter.Quality = 100;
MovWriter.FrameRate = 10;
open(MovWriter);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
fprintf('Getting parameters...\n');
fid = fopen('../data/parameters.txt','r');
dimension = extract_data(fid,'$Dimension','%d',1);
nbNodesByRow = extract_data(fid,'$NodesByRow','%d',1);
nbNodesByCol = extract_data(fid,'$NodesByCol','%d',1);
nbStages = extract_data(fid,'$NumberStages','%d',1);
nbSteps = extract_data(fid,'$NbStepsPerStage','%d',1);
dt = extract_data(fid,'$TimeStepSize','%f',1);
nbCells = extract_data(fid,'$NumberCells','%d',1);
nbTypes = extract_data(fid,'$NumberTypes','%d',1);
nbNodes = nbNodesByRow*nbNodesByCol;
if dimension==3
    nbNodes = nbNodes*nbNodesByRow;
end
if nbStages == 1
    nbSigmas = 0.5*nbTypes*(nbTypes+1);
    tensionStages = extract_data(fid,'$SigmaPerStage','%f',nbSigmas)';
else
    tensionStages = readmatrix('output/sigmavalues.txt');
end
cellLabel = extract_data(fid,'$CellLabel','%c',nbTypes);
adhten = extract_data(fid,'$DisplayType','%c',7);
Cmap = reshape(extract_data(fid,'$Color','%f',nbTypes*3),3,nbTypes)';
fclose(fid);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
fprintf('Getting initial configuration data...\n');
fid = fopen('../data/initconfig.txt','r'); fgetl(fid);

typeOfCell = zeros(nbCells,1);
for i=1:nbCells
    tmp = fscanf(fid, '%d %d\n',2);
    typeOfCell(i) = tmp(2);
end
outFile = zeros(nbNodes,1); fgetl(fid);
for i=1:nbNodes
    outFile(i) = fscanf(fid, '%d\n',1);
end
fclose(fid);

scale = (nbCells-1)/(nbTypes-1);
X = linspace(0,1,nbNodesByCol); 
Y = linspace(0,(nbNodesByRow-1)/(nbNodesByCol-1),nbNodesByRow);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
for t=0:nbStages*nbSteps
    fprintf('Making frame %d...\n',t);
%    makeFrames(t,dt,dimension,MovWriter);
    f = figure('visible','off');
    hold on;
    outFile = sprintf('output/sol%04d.txt',t);
    name = sprintf('Level Set-based Scheme:       t = %f', t*dt);

    if dimension == 2
        U = reshape(readmatrix(outFile),nbNodesByCol,nbNodesByRow)';
        Z = typeOfCell(U+1).*scale;
        imagesc(X,Y,Z,[0 max((0:nbTypes-1).*scale)]);
        contour(X,Y,U,0:nbCells-1, 'k', 'LineWidth',1.0);
        colormap(Cmap(1:nbTypes,:));
        set(gca,'xtick',[],'ytick',[],'xticklabel',[],'yticklabel',[]);
        display_colorlegend(nbTypes,scale,cellLabel);
        axis equal;
    end
    if dimension==3
        [x,y,z] = meshgrid(X);
        U = reshape(readmatrix(outFile)+1,nbNodesByRow,nbNodesByRow,nbNodesByRow);
        for q=1:nbCells-1
            v = smooth3(U==q,'box',5);
            fv=isosurface(x,y,z,v,0.5);
            fq = patch(fv,'FaceColor',Cmap(q,:),'EdgeColor','none');
            isonormals(x,y,z,v,fq);
        end
        colormap(Cmap(1:nbTypes,:));
        colorbar('off');
        alpha(.75)
        axis([0 1 0 1 0 1]);
        daspect([1 1 1]);
        view([4 -2 1]);
        set(gca,'xtick',[],'ytick',[],'ztick',[]);
        set(gca,'xticklabel',[],'yticklabel',[],'zticklabel',[]);
        box off;
        light;
        lighting phong;
        camlight('left');
    end

    sigma = display_sigma(t,nbStages,nbTypes,cellLabel,tensionStages,adhten);
    xlabel(sigma,'Fontsize',8);
    title(name);
    hold off;
    writeVideo(MovWriter, getframe(f));
    close(f);
end
close(MovWriter);

disp('Done!');