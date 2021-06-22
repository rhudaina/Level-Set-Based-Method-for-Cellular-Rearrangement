function display_colorlegend(nbTypes,scale,cellLabel)
    % this subroutine prints the color legend for cell types
    
    val = max((0:nbTypes-1).*scale)./(2*nbTypes);
    label = cell(1,2);
    for i = 1:nbTypes
        label{i} = cellLabel(i);
    end
    colorbar('Ticks',val.*(1:2:2*nbTypes),'TickLabels',label);
end
