function sg = display_sigma(p,nbStages,nbTypes,cl,tensionStages,adhten)
    % this subroutine prints tension or adhesion values by cell types
    
    if nbStages==1
        p = 0;
    end
    if strcmp(adhten,'adhsion')==1
        gk = 'alpha';   nb = 1./tensionStages;
    end
    if strcmp(adhten,'tension')==1
        gk = 'sigma';      nb = tensionStages;
    end
    k = 1;
    for j=1:nbTypes
        for i=1:j
            if k==1
                sg = sprintf('\\%s_{%s%s} = %.3f', gk, cl(i), cl(j), nb(p+1,k));
            else
                sg = strcat(sg, sprintf('   \\%s_{%s%s} = %.3f', gk, cl(i), cl(j), nb(p+1,k)));
            end
            k = k+1;
        end
    end
end
