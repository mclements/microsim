window.onload = function () {
	var run = Module.cwrap('simulation_run', 'number', ['number'])
	var printSummary = Module.cwrap('simulation_print_summary', 'number', ['number'])
	var printVisits = Module.cwrap('simulation_print_visits', 'number', ['number'])

	var preloaderRenderer = Tempo.prepare("preloader")
	var summaryRenderer = Tempo.prepare("summary")
	var chartTitleRenderer = Tempo.prepare("chart-title")
	// var legendsRenderer = Tempo.prepare("chart-legends")
	var execTimeRenderer = Tempo.prepare("execution-time")
	var errorRenderer = Tempo.prepare("error")

	var paperWidth = document.getElementById("chart").offsetWidth
	var paperHeight = Math.floor(paperWidth * 0.5)
	// var paper = Raphael(document.getElementById("chart"), paperWidth, paperHeight)

	// function legends(states)
	// {
	// 	var result = []
	// 	for (var i in states) {
	// 		var hsb = Raphael.g.colors[i].match(/(\d*\.\d+)/g) //"hsb(h, s, b)" -> [h, s, b]
	// 		var rgb = Raphael.hsb2rgb(hsb[0], hsb[1], hsb[2])
	// 		result.push({state: states[i], color: rgb.hex})
	// 	}
	// 	return result
	// }


	function displaySummary(json, executionTime)
	{
		var result = JSON.parse(json)
		summaryRenderer.render(result)
		execTimeRenderer.render([[executionTime / 1000]])
		preloaderRenderer.clear()
		document.getElementById("output").style.display = "block"
	}


	function displayChart(json, n)
	{
	    var ageMax = 150;
	    var result = JSON.parse(json);
	    var myData = [];
	    
	    for (var state in result) {
		var freqs = result[state].map(function (x) { return x / n })
		var localData = [];
		for (var i = 0; i <= ageMax; i++) {
		    localData.push({x:i,y:freqs[i]});
		}
		myData.push({key: state, values: localData});
	    }

	    chartTitleRenderer.render([["Estimated probability per age"]]);

	    nv.addGraph(function() {
		var chart = nv.models.lineChart()
		    .color(d3.scale.category10().range())
		    .x(function(d) { return d.x })
                    .y(function(d) { return d.y }); 	

		chart.xAxis
		    .axisLabel("Age (years)");
		
		d3.select("#chart svg")
		    .attr("width",500)
		    .attr("height",300)
		    .datum(myData)
		    .transition().duration(500).call(chart);

		nv.utils.windowResize( chart.update );

		return chart;
	    });
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
		
		window.console.log = function (str) { displayChart(str, n) }
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
			preloaderRenderer.render([["Executing..."]])
			//set timeout to force page update
			setTimeout(function() { doSubmit(n) }, 100)
		} else {
			errorRenderer.render([[n]])
		}	
		event.preventDefault()
	}
	
	
	function init()
	{
		//register event handlers
		var form = document.getElementById("input")
		form.addEventListener("submit", submit, false)
	
		//hide output
		document.getElementById("output").style.display = "none"
		
		document.getElementById("population-size").select()
	}
	
	init()
}
