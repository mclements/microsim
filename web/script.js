window.onload = function () {
	var run = Module.cwrap('simulation_run', 'number', ['number'])
	var printSummary = Module.cwrap('simulation_print_summary', 'number', ['number'])
	var printVisits = Module.cwrap('simulation_print_visits', 'number', ['number'])

	var resultRenderer = Tempo.prepare("result-table")
	var execTimeRenderer = Tempo.prepare("execution-time")
	var errorRenderer = Tempo.prepare("error")

	var paperWidth = document.getElementsByTagName("body")[0].offsetWidth
	var paperHeight = Math.floor(paperWidth * 0.5)
	var paper = Raphael(document.getElementById("chart"), paperWidth, paperHeight)

	function displaySummary(json, executionTime)
	{
		var result = JSON.parse(json)
		resultRenderer.render(result)
		execTimeRenderer.render([[executionTime / 1000]])
		document.getElementById("preloader").style.display = "none"
		document.getElementById("output").style.display = "block"
	}


	function displayVisits(json, n)
	{
		var ageMax = 150
		var ofs = 30
		var result = JSON.parse(json)

		var xs = []
		for (var i = 0; i <= ageMax; i++) {
			xs.push(i)
		}
		
		var ys = []
		for (var state in result) {
			var freqs = result[state].map(function (x) { return x / n })
			var slicedFreqs = freqs.slice(0, ageMax + 1)
			ys.push(slicedFreqs)
		}

		var opts = {axis: "0 0 1 1", axisxstep: 20, axisystep: 10}
		paper.clear()
		paper.linechart(ofs, ofs, paper.width - ofs*2, paper.height - ofs*2, xs, ys, opts)
	}


	function doSubmit(n)
	{
		var asJson = 1

		var start = new Date()
		run(n)
		executionTime = new Date() - start

		var defaultLogger = window.console.log

		window.console.log = function (str) { displaySummary(str, executionTime) }
		printSummary(asJson)
		
		window.console.log = function (str) { displayVisits(str, n) }
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
