<template>
    <div class="row">
      <div class="gauge-block">
        <SemiGauge v-if="gas !== null" :value="gas" :max="10000" :min="200" unit="ppm" label="가스" type="gas" />
      </div>
      <div class="gauge-block">
        <SemiGauge v-if="moisture !== null" :value="moisture" :max="100" :min="0" unit="%" label="토양수분" type="moisture" />
      </div>
    </div>
  </template>
  
  <script setup>
  import SemiGauge from './SemiGauge.vue';
  defineProps(['gas', 'moisture']);
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
  
  /* semi gauge 내부 텍스트 여백 */
  :deep(.gauge-value) {
    margin-bottom: 8px;
    white-space: nowrap;
  }
  </style>
  