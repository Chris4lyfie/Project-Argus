#pragma once

const char* htmlPage = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>Project Argus</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link href="https://fonts.googleapis.com/css2?family=Orbitron:wght@700&display=swap" rel="stylesheet">
  <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
  <style>
    * { box-sizing: border-box; margin: 0; padding: 0; }
    body { font-family: Arial, sans-serif; background: #0a0a0a; color: #e0e0e0; padding: 24px; }
    h1 { font-family: 'Orbitron', sans-serif; text-align: center; font-size: 1.8rem; margin-bottom: 6px; color: #58a6ff; letter-spacing: 4px; }
    #updated { text-align: center; font-size: 0.8rem; color: #555; margin-bottom: 20px; }
    .grid { display: grid; grid-template-columns: 1fr 1fr; gap: 16px; margin-bottom: 16px; }
    .chart-container { background: #111; border: 2px solid #222; border-radius: 10px; padding: 16px; transition: border-color 0.5s; }
    canvas { max-height: 200px; }
  </style>
</head>
<body>
  <h1>PROJECT ARGUS</h1>
  <p id="updated">Waiting for data...</p>
  <div class="grid">
    <div class="chart-container" id="box0"><canvas id="c0"></canvas></div>
    <div class="chart-container" id="box1"><canvas id="c1"></canvas></div>
    <div class="chart-container" id="box2"><canvas id="c2"></canvas></div>
    <div class="chart-container" id="box3"><canvas id="c3"></canvas></div>
    <div class="chart-container" id="box4"><canvas id="c4"></canvas></div>
    <div class="chart-container" id="box5"><canvas id="c5"></canvas></div>
  </div>
  <div class="chart-container" id="boxH"><canvas id="humid"></canvas></div>

  <script>
    const MAX = 30;
    const labels = [];
    let pollCount = 0;

    const liveValuePlugin = {
      id: 'liveValue',
      afterDatasetsDraw(chart) {
        const ctx = chart.ctx;
        chart.data.datasets.forEach((ds, i) => {
          const meta = chart.getDatasetMeta(i);
          if (!ds.data.length || meta.hidden) return;
          const last = meta.data[ds.data.length - 1];
          const val  = ds.data[ds.data.length - 1];
          if (last === undefined || val === undefined) return;
          ctx.save();
          ctx.fillStyle    = ds.borderColor;
          ctx.font         = 'bold 11px Arial';
          ctx.textAlign    = 'left';
          ctx.textBaseline = 'middle';
          ctx.fillText(val.toFixed(1), last.x + 6, last.y);
          ctx.restore();
        });
      }
    };
    Chart.register(liveValuePlugin);

    const sensors = [
      { id:'c0', label:'Argus Temp', color:'#58a6ff', key:'sht_temp' },
      { id:'c1', label:'Sensor #1',  color:'#f78166', key:'ds0' },
      { id:'c2', label:'Sensor #2',  color:'#3fb950', key:'ds1' },
      { id:'c3', label:'Sensor #3',  color:'#d2a8ff', key:'ds2' },
      { id:'c4', label:'Sensor #4',  color:'#ffa657', key:'ds3' },
      { id:'c5', label:'Sensor #5',  color:'#79c0ff', key:'ds4' },
    ];

    function tempColor(val) {
      if (val >= 105) return '#f78166';
      if (val >= 90)  return '#ffa657';
      return '#222';
    }

    function makeOpts(title, yExtra) {
      return {
        responsive: true, animation: false,
        layout: { padding: { right: 40 } },
        plugins: {
          legend: { display: false },
          title:  { display: true, text: title, color: '#e0e0e0' }
        },
        scales: {
          x: {
            ticks: {
              color: '#555',
              maxRotation: 0,
              autoSkip: false,
              callback: function(val) { return labels[val] || ''; }
            },
            grid: { color: '#1a1a1a' }
          },
          y: { ...yExtra, ticks: { color: '#555' }, grid: { color: '#1a1a1a' } }
        }
      };
    }

    const charts = sensors.map(s => new Chart(document.getElementById(s.id), {
      type: 'line',
      data: { labels, datasets: [{ label: s.label, data: [], borderColor: s.color, tension: 0.3, fill: false, pointRadius: 3, pointHoverRadius: 5 }] },
      options: makeOpts(s.label, { min: 65, max: 120 })
    }));

    const humidChart = new Chart(document.getElementById('humid'), {
      type: 'line',
      data: { labels, datasets: [{ label: 'Humidity (%)', data: [], borderColor: '#48cae4', tension: 0.3, fill: false, pointRadius: 3, pointHoverRadius: 5 }] },
      options: makeOpts('Humidity (%)', { min: 0, max: 100 })
    });

    function addData(d) {
      pollCount++;
      const now = new Date().toLocaleTimeString();
      if (labels.length >= MAX) {
        labels.shift();
        charts.forEach(c => c.data.datasets[0].data.shift());
        humidChart.data.datasets[0].data.shift();
      }
      labels.push(pollCount % 10 === 0 ? now : '');

      sensors.forEach((s, i) => {
        const val = d[s.key];
        charts[i].data.datasets[0].data.push(val);
        charts[i].update();
        document.getElementById('box' + i).style.borderColor = tempColor(val);
      });

      humidChart.data.datasets[0].data.push(d.humidity);
      humidChart.update();
      document.getElementById('updated').textContent = 'Last updated: ' + now;
    }

    function fetchData() {
      fetch('/data').then(r => r.json()).then(addData).catch(console.error);
    }
    setInterval(fetchData, 2000);
    fetchData();
  </script>
</body>
</html>
)rawliteral";
