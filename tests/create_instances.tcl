#!/usr/bin/tclsh
# TCL script to create instances
# 
set tcl_precision 17

if {$argc < 2} {
	puts "Usage: create_instances.tcl vars clauses"
	exit
	}
#else...
set vars [lindex $argv 0]
set clauses [lindex $argv 1]
set lit_per_cla 4

set ntests 1


for {set iter 1} {$iter <= $ntests} {incr iter 1} {
	set outfile "sel${lit_per_cla}_${vars}_${clauses}_${iter}.cnf"
	set ofd  [open $outfile w]
	set result [exec ./makewff -seed $iter -cnf $lit_per_cla $vars $clauses]
    puts $ofd "c created by makewff from Bart Selman"    
    puts $ofd $result
	close $ofd
}

