window.onload = function () {
	var run = Module.cwrap('simulation_run', 'number', ['number'])
	var print = Module.cwrap('simulation_print', 'number', ['number'])
	var resultRenderer = Tempo.prepare("result")
	var execTimeRenderer = Tempo.prepare("execution-time")
	var defaultLogger = window.console.log

	var executionTime //in seconds

	function displayResult(json)
	{
		var result = JSON.parse(json)
		resultRenderer.render(result)
		execTimeRenderer.render([[executionTime]])
	}


	function submit(event)
	{
		var asJson = 1
		
		var n = document.getElementById("population-size").value
		var start = new Date()	
		run(n)
		executionTime = (new Date() - start) / 1000;

		window.console.log = displayResult
		print(asJson)
		window.console.log = defaultLogger
	
		event.preventDefault()
	}

	//register event handlers
	var form = document.getElementById("input")
	form.addEventListener("submit", submit, false)
}
