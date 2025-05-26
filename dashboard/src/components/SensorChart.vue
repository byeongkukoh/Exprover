<template>
  <div class="chart-wrapper">
    <div class="chart-container">
      <h3>온도 (°C)</h3>
      <canvas ref="tempChart"></canvas>
    </div>
    <div class="chart-container">
      <h3>습도 (%)</h3>
      <canvas ref="humiChart"></canvas>
    </div>
    <div class="chart-container">
      <h3>기압 (hPa)</h3>
      <canvas ref="presChart"></canvas>
    </div>
    <div class="chart-container">
      <h3>가스 (ppm)</h3>
      <canvas ref="gasChart"></canvas>
    </div>
    <div class="chart-container">
      <h3>토양수분 (%)</h3>
      <canvas ref="moisChart"></canvas>
    </div>
  </div>
</template>

<script setup>
import { ref, onMounted } from 'vue';
import Chart from 'chart.js/auto';
import { fetchRecentSensorData } from './SensorDatas.js';

const tempChart = ref(null);
const humiChart = ref(null);
const presChart = ref(null);
const gasChart = ref(null);
const moisChart = ref(null);

onMounted(async () => {
  const rawData = await fetchRecentSensorData('EXP');

  const cleanedData = rawData.filter(d =>
    d.time?.seconds &&
    typeof d.temp === 'number' &&
    typeof d.humi === 'number' &&
    typeof d.pres === 'number' &&
    typeof d.gas === 'number' &&
    typeof d.mois === 'number'
  );

  const labels = cleanedData.map(d =>
    new Date(d.time.seconds * 1000).toLocaleTimeString()
  );

  const createLineChart = (canvasRef, label, data, color) => {
    new Chart(canvasRef.value, {
      type: 'line',
      data: {
        labels,
        datasets: [
          {
            label,
            data,
            borderColor: color,
            backgroundColor: `${color}22`,
            fill: true,
            tension: 0.3
          }
        ]
      },
      options: {
        responsive: true,
        maintainAspectRatio: false,
        plugins: {
          legend: { display: true },
        },
        scales: {
          y: {
            beginAtZero: false
          }
        }
      }
    });
  };

  createLineChart(tempChart, '온도 (°C)', cleanedData.map(d => d.temp), '#f44336');
  createLineChart(humiChart, '습도 (%)', cleanedData.map(d => d.humi), '#2196f3');
  createLineChart(presChart, '기압 (hPa)', cleanedData.map(d => d.pres), '#9c27b0');
  createLineChart(gasChart, '가스 (ppm)', cleanedData.map(d => d.gas), '#ff9800');
  createLineChart(moisChart, '토양수분 (%)', cleanedData.map(d => d.mois), '#4caf50');
});
</script>



  
<style scoped>
.chart-wrapper {
  display: flex;
  flex-direction: column;
  gap: 30px;
  padding: 20px;
}

.chart-container {
  background: white;
  padding: 16px;
  border-radius: 12px;
  box-shadow: 0 0 6px rgba(0, 0, 0, 0.05);
  max-width: 800px;
  height: 400px;
  margin: 0 auto;
}
canvas {
  width: 100% !important;
  height: 100% !important;
}

</style>