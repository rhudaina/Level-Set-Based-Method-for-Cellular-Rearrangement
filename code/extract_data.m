function data = extract_data(fid, info, fmtspec, N)
    % this subroutine extracts specified information from data file
    
    tline = fgetl(fid);
    lineCounter = 1;
    while ischar(tline)
        if strcmp(tline, info)
            break;
        end
        tline = fgetl(fid);
        lineCounter = lineCounter + 1;
    end
    data = fscanf(fid, fmtspec, N);
end
