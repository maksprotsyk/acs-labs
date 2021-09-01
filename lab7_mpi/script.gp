set terminal gif animate delay 50

# output_file
set output "tesst.gif"

set view map scale 1
set style data lines

set title "MPI lab"

input_file = "file.txt"

# input file
stats input_file

cols_num = STATS_columns
rows_num = STATS_records / STATS_blocks

set xrange [ -0.5 : cols_num - 0.5 ]
set yrange [ -0.5 : rows_num - 0.5 ]


set cblabel "Temperature" 
set cbrange [ 0 : 100 ]
set palette defined (0 "blue", 25 "cyan", 50 "yellow", 100 "red")


do for [i=0:STATS_blocks-1] {
plot input_file using 1:(rows_num - $2 - 1):3 index i matrix w image;
}