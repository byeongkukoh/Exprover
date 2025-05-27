<template>
  <div class="bar-container">
    <div class="bar" :style="barStyle">
      <span class="value-text">{{ value }} {{ unit }}</span>
    </div>
    <div class="label">{{ label }}</div>
  </div>
</template>

<script setup>
import { defineProps, computed } from 'vue';

const props = defineProps({
  value: Number,
  max: Number,
  min: Number,
  unit: String,
  label: String,
  type: {
    type: String,
    default: 'default'  // 'temperature', 'humidity', 'pressure', 'moisture', 'gas'
  }
});

// ✅ 색상 결정 함수: 값의 비율(ratio)에 따라 색상 지정
const getBarColor = (type, ratio) => {
  if (type === 'humidity') {
    // ✅ 습도 (중간이 적정)
    if (ratio < 0.3) return '#f44336';      // 빨강 (건조함)
    if (ratio > 0.8) return '#2196f3';      // 파랑 (습함)
    else if (ratio < 0.4 || ratio > 0.6) return '#ffc107'; // 노랑 (주의)
    else return '#4caf50';                                // 초록 (안정)
  } else if (type === 'pressure') {
    // ✅ 기압 (중간이 적정)
    if (ratio > 0.6) return '#f44336';                     // 빨강 (고기압)
    else if (ratio < 0.4) return '#2196f3'                 // 파랑 (저기압)
    else return '#4caf50';                                 // 초록 (적정 기압)
  } else if (type === 'temperature') {
    // ✅ 온도 (중간이 안정)
    if (ratio < 0.2 || ratio > 0.7) return '#f44336'; // 빨강 (위험)
    else if (ratio < 0.3) return '#ffc107';           // 노랑 (주의)
    else return '#4caf50';                            // 초록 (안정)
  }
};

// ✅ 바 높이와 색상 계산
const barStyle = computed(() => {
  if (typeof props.value !== 'number' || isNaN(props.value)) {
    return {
      height: '0%',
      backgroundColor: '#ccc' // 기본 회색
    };
  }

  const normalizedValue = Math.min(Math.max(props.value, props.min), props.max);
  const valueRatio = (normalizedValue - props.min) / (props.max - props.min);

  return {
    height: `${valueRatio * 100}%`,
    backgroundColor: getBarColor(props.type, valueRatio)
  };
});
</script>

<style scoped>
.bar-container {
  position: relative;
  height: 150px;
  width: 60px;
  background: #e0e0e0;
  display: flex;
  align-items: flex-end;
  justify-content: center;
  margin: 0 10px;
}

.bar {
  width: 100%;
  position: relative;
  display: flex;
  justify-content: center;
  align-items: flex-start;
}

.value-text {
  position: absolute;
  top: -20px;
  left: 50%;
  transform: translateX(-50%);
  font-size: 14px;
  color: black;
  white-space: nowrap;
  min-width: 80px;
  text-align: center;
}

.label {
  text-align: center;
  margin-top: 10px;
  font-weight: bold;
}
</style>
