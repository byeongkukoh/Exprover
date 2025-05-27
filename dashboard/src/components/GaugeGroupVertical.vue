<template>
    <div class="row">
      <div class="gauge-block">
        <VerticalBarGauge v-if="temperature !== null" :value="temperature" :max="50" :min="-30" unit="°C" type="temperature" />
        <span class="label">온도</span>
      </div>
      <div class="gauge-block">
        <VerticalBarGauge v-if="humidity !== null" :value="humidity" :max="100" :min="0" unit="%" type="humidity" />
        <span class="label">습도</span>
      </div>
      <div class="gauge-block">
        <VerticalBarGauge v-if="pressure !== null" :value="pressure" :max="1050" :min="950" unit="hPa" type="pressure" />
        <span class="label">기압</span>
      </div>
    </div>
  </template>
  
  <script setup>
  import VerticalBarGauge from './VerticalBarGauge.vue';
  defineProps(['temperature', 'humidity', 'pressure']);
  </script>
  
  <style scoped>
  .row {
    display: flex;
    justify-content: center;
    gap: 50px;
    flex-wrap: wrap;
  }
  
  .gauge-block {
    display: flex;
    flex-direction: column;
    align-items: center;
    min-width: 120px;
    position: relative;
  }
  
  .label {
    margin-top: 12px;
    font-weight: 500;
    font-size: 14px;
  }
  
  /* vertical bar gauge 내부 텍스트 여백 */
  :deep(.bar-value) {
    margin-bottom: 4px;
    white-space: nowrap;
  }
  </style>
  