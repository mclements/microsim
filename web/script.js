window.onload = function () {
	var run = Module.cwrap('simulation_run', 'number', ['number'])
	var print = Module.cwrap('simulation_print', 'number', ['number'])

	var resultRenderer = Tempo.prepare("result-table")
	var execTimeRenderer = Tempo.prepare("execution-time")
	var errorRenderer = Tempo.prepare("error")

	var defaultLogger = window.console.log
	var executionTime //in seconds

	function displayResult(json)
	{
		var result = JSON.parse(json)
		resultRenderer.render(result)
		execTimeRenderer.render([[executionTime]])
		document.getElementById("output").style.display = "block"
	}


	function submit(event)
	{
		var asJson = 1
		
		//clear previous output		
		document.getElementById("output").style.display = "none"
		errorRenderer.clear()
		
		var n = document.getElementById("population-size").value
		if (n.match(/^[0-9]+$/)) {
			var start = new Date()	
			run(n)
			executionTime = (new Date() - start) / 1000;

			window.console.log = displayResult
			print(asJson)
			window.console.log = defaultLogger
		} else {
			errorRenderer.render([[n]])
		}	
		event.preventDefault()
	}

	//register event handlers
	var form = document.getElementById("input")
	form.addEventListener("submit", submit, false)
	
	//hide output template
	document.getElementById("output").style.display = "none"
}
