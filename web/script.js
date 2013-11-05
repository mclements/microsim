var resultRenderer
var run
var print

function displayResult(json)
{
	var result = JSON.parse(json)
	resultRenderer.render(result)
}


function submit(event)
{
	var n = document.getElementById("population-size").value
	run(n)

	var defaultLogger = window.console.log
	window.console.log = displayResult
	var asJson = 1
	print(asJson)
	window.console.log = defaultLogger
	
	event.preventDefault()
}


function init()
{
	//initialize global variables
	resultRenderer = Tempo.prepare("result")
	run = Module.cwrap('simulation_run', 'number', ['number'])
	print = Module.cwrap('simulation_print', 'number', ['number'])

	//register event handlers
	var form = document.getElementById("input")
	form.addEventListener("submit", submit, false)
}

window.onload = init
