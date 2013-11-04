function main()
{
	var run = Module.cwrap('simulation_run', 'number', ['number'])
	var print = Module.cwrap('simulation_print', 'number', ['number'])
	run(100)
	print()
}

window.onload = main
