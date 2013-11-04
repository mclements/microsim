function log(str)
{
	var result = JSON.parse(str)
	alert(result)
}


function main()
{
	var run = Module.cwrap('simulation_run', 'number', ['number'])
	var print = Module.cwrap('simulation_print', 'number', ['number'])
	run(100)

	var savedLog = window.console.log
	window.console.log = log
	var asJson = 1
	print(asJson)
	window.console.log = savedLog
}

window.onload = main
