window.onload = function () {
	var run = Module.cwrap('simulation_run', 'number', ['number'])
	var print = Module.cwrap('simulation_print', 'number', ['number'])
	var resultRenderer = Tempo.prepare("result")
	var defaultLogger = window.console.log

	function displayResult(json)
	{
		var result = JSON.parse(json)
		resultRenderer.render(result)
	}


	function submit(event)
	{
		var asJson = 1
		
		var n = document.getElementById("population-size").value		
		run(n)

		window.console.log = displayResult
		print(asJson)
		window.console.log = defaultLogger
	
		event.preventDefault()
	}

	//register event handlers
	var form = document.getElementById("input")
	form.addEventListener("submit", submit, false)
}
