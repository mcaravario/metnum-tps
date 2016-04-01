for i=1:41


	for j=1:10
    
    	
    	s = sprintf('./s%d/%d.pgm', i, j)
	    A = imread(s)

    	s2 = sprintf('./s%d/%d.in', i, j)
    	fi = fopen(s2, 'wt')

    	for k=1:112
        	fprintf(fi, '%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n', A(k,1),A(k,2),A(k,3),A(k,4),A(k,5),A(k,6),A(k,7),A(k,8),A(k,9),A(k,10),A(k,11),A(k,12),A(k,13),A(k,14),A(k,15),A(k,16),A(k,17),A(k,18),A(k,19),A(k,20),A(k,21),A(k,22),A(k,23),A(k,24),A(k,25),A(k,26),A(k,27),A(k,28),A(k,29),A(k,30),A(k,31),A(k,32),A(k,33),A(k,34),A(k,35),A(k,36),A(k,37),A(k,38),A(k,39),A(k,40),A(k,41),A(k,42),A(k,43),A(k,44),A(k,45),A(k,46),A(k,47),A(k,48),A(k,49),A(k,50),A(k,51),A(k,52),A(k,53),A(k,54),A(k,55),A(k,56),A(k,57),A(k,58),A(k,59),A(k,60),A(k,61),A(k,62),A(k,63),A(k,64),A(k,65),A(k,66),A(k,67),A(k,68),A(k,69),A(k,70),A(k,71),A(k,72),A(k,73),A(k,74),A(k,75),A(k,76),A(k,77),A(k,78),A(k,79),A(k,80),A(k,81),A(k,82),A(k,83),A(k,84),A(k,85),A(k,86),A(k,87),A(k,88),A(k,89),A(k,90),A(k,91),A(k,92));
    	end

    	fclose(fi)

	end
end
