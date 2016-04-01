for i=1:41


	for j=1:10
    
    	
    	s = sprintf('./s%d/%d.pgm', i, j)
	    A = imread(s)

    	s2 = sprintf('./s%d/%d.in', i, j)
    	fi = fopen(s2, 'wt')

    	for k=1:28
        	fprintf(fi, '%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n', A(k,1),A(k,2),A(k,3),A(k,4),A(k,5),A(k,6),A(k,7),A(k,8),A(k,9),A(k,10),A(k,11),A(k,12),A(k,13),A(k,14),A(k,15),A(k,16),A(k,17),A(k,18),A(k,19),A(k,20),A(k,21),A(k,22),A(k,23));
    	end

    	fclose(fi)

	end
end
