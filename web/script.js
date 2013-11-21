window.onload = function () {
	var run = Module.cwrap('simulation_run', 'number', ['number'])
	var printSummary = Module.cwrap('simulation_print_summary', 'number', ['number'])
	var printVisits = Module.cwrap('simulation_print_visits', 'number', ['number'])

	var resultRenderer = Tempo.prepare("result-table")
	var execTimeRenderer = Tempo.prepare("execution-time")
	var errorRenderer = Tempo.prepare("error")

	var defaultLogger = window.console.log
	var executionTime //in milliseconds

	function displaySummary(json)
	{
		var result = JSON.parse(json)
		resultRenderer.render(result)
		execTimeRenderer.render([[executionTime / 1000]])
		document.getElementById("preloader").style.display = "none"
		document.getElementById("output").style.display = "block"
	}


	function displayVisits(json)
	{
		var result = JSON.parse(json)
		/*to be implemented*/
	}


	function doSubmit(n)
	{
		var asJson = 1

		var start = new Date()
		run(n)
		executionTime = new Date() - start

		window.console.log = displaySummary
		printSummary(asJson)
		
		window.console.log = displayVisits
		printVisits(asJson)
		
		window.console.log = defaultLogger
	}


	function submit(event)
	{
		//clear previous output
		document.getElementById("output").style.display = "none"
		errorRenderer.clear()
		
		var n = document.getElementById("population-size").value
		if (n.match(/^[0-9]+$/)) {
			document.getElementById("preloader").style.display = "inline"
			//set timeout to force page update
			setTimeout(function() { doSubmit(n) }, 100)
		} else {
			errorRenderer.render([[n]])
		}	
		event.preventDefault()
	}

	//register event handlers
	var form = document.getElementById("input")
	form.addEventListener("submit", submit, false)
	
	//hide preloader and output
	document.getElementById("preloader").style.display = "none"
	document.getElementById("output").style.display = "none"
}
