<script setup>
import ChatBot from "./components/ChatBot.vue"
import RainPredictor from "./components/RainPredictor.vue"
import { ref, onMounted } from 'vue';
import { fetchSensorData } from './components/SensorData.js';
import GaugeGroupVertical from './components/GaugeGroupVertical.vue';
import GaugeGroupSemi from './components/GaugeGroupSemi.vue';
import SensorChart from './components/SensorChart.vue';

const selectedCar = ref('EXP');

const temperature = ref(null);
const humidity = ref(null);
const pressure = ref(null);
const gas = ref(null);
const moisture = ref(null);

const updateSensorValues = async () => {
  const data = await fetchSensorData(selectedCar.value);
  const safe = (v) => (typeof v === 'number' && !isNaN(v)) ? v : null;

  temperature.value = safe(data.temp);
  humidity.value = safe(data.humi);
  pressure.value = safe(data.pres);
  gas.value = safe(data.gas);
  moisture.value = safe(data.mois);
};

onMounted(() => {
  updateSensorValues();
  setInterval(updateSensorValues, 360000);
});
</script>

<template>
  <div class="container">
    <div class="header">
      <label>차량 선택</label>
      <select v-model="selectedCar">
        <option value="EXP">EXP</option>
        <option value="ROVER">ROVER</option>
      </select>
    </div>

    <div class="vertical-container">
      <GaugeGroupVertical
        :temperature="temperature"
        :humidity="humidity"
        :pressure="pressure"
      />
      <GaugeGroupSemi
        :gas="gas"
        :moisture="moisture"
      />
    </div>
  </div>

  <SensorChart />

  <div>
    <ChatBot />
  </div>
  <div>
    <RainPredictor />
  </div>
</template>

<style scoped>
.logo {
  height: 6em;
  padding: 1.5em;
  will-change: filter;
  transition: filter 300ms;
}

.logo:hover {
  filter: drop-shadow(0 0 2em #646cffaa);
}

.logo.vue:hover {
  filter: drop-shadow(0 0 2em #42b883aa);
}
</style>
