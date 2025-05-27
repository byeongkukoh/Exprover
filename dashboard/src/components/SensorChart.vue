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
import { ref, watch, onMounted } from "vue";
import Chart from "chart.js/auto";
import { fetchRecentSensorData } from "./SensorDatas.js";

const tempChart = ref(null);
const humiChart = ref(null);
const presChart = ref(null);
const gasChart = ref(null);
const moisChart = ref(null);

const chartInstances = {
  temp: null,
  humi: null,
  pres: null,
  gas: null,
  mois: null,
};

const props = defineProps(["selectedCar"]);

onMounted(() => {
  drawLineChart(props.selectedCar); // 최초 로딩 시
});

watch(
  () => props.selectedCar,
  (newVal) => {
    drawLineChart(newVal); // 차량 선택 변경 시
  }
);

const computeYAxisRange = (data, paddingRatio = 0.6, stepCount = 5) => {
  const min = Math.min(...data);
  const max = Math.max(...data);
  const range = max - min || 1;
  const padding = range * paddingRatio;
  let yMin = Math.floor((min - padding) / 10) * 10;
  let yMax = Math.ceil((max + padding) / 10) * 10;
  if (yMin === yMax) {
    yMin -= 2;
    yMax += 2;
  }
  const step = Math.ceil((yMax - yMin) / stepCount);
  return { yMin, yMax, step };
};

const createLineChart = (
  canvasRef,
  chartKey,
  label,
  data,
  color,
  { yMin, yMax, step }
) => {
  if (chartInstances[chartKey]) {
    chartInstances[chartKey].destroy();
  }

  const ctx = canvasRef.value.getContext("2d");

  chartInstances[chartKey] = new Chart(ctx, {
    type: "line",
    data: {
      labels: data.map((_, i) => i + 1),
      datasets: [
        {
          label,
          data,
          borderColor: color,
          backgroundColor: `${color}22`,
          fill: true,
          tension: 0.3,
        },
      ],
    },
    options: {
      responsive: true,
      maintainAspectRatio: false,
      plugins: {
        legend: { display: true },
      },
      scales: {
        y: {
          min: yMin,
          max: yMax,
          ticks: {
            stepSize: step,
          },
        },
      },
    },
  });
};

const drawLineChart = async (carType) => {
  const rawData = await fetchRecentSensorData(carType);
  const cleanedData = rawData.filter(
    (d) =>
      d.time?.seconds &&
      typeof d.temp === "number" &&
      typeof d.humi === "number" &&
      typeof d.pres === "number" &&
      typeof d.gas === "number" &&
      typeof d.mois === "number"
  );

  const tempData = cleanedData.map((d) => d.temp);
  const humiData = cleanedData.map((d) => d.humi);
  const presData = cleanedData.map((d) => d.pres);
  const gasData = cleanedData.map((d) => d.gas);
  const moisData = cleanedData.map((d) => d.mois);

  createLineChart(
    tempChart,
    "temp",
    "온도",
    tempData,
    "#f44336",
    computeYAxisRange(tempData)
  );
  createLineChart(
    humiChart,
    "humi",
    "습도",
    humiData,
    "#2196f3",
    computeYAxisRange(humiData)
  );
  createLineChart(
    presChart,
    "pres",
    "기압",
    presData,
    "#9c27b0",
    computeYAxisRange(presData)
  );
  createLineChart(
    gasChart,
    "gas",
    "가스",
    gasData,
    "#ff9800",
    computeYAxisRange(gasData)
  );
  createLineChart(
    moisChart,
    "mois",
    "토양수분",
    moisData,
    "#4caf50",
    computeYAxisRange(moisData)
  );
};

onMounted(() => {
  drawLineChart("EXP");
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
