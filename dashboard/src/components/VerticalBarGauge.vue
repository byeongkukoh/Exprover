<template>
  <div class="bar-container">
    <div class="bar" :style="barStyle">
      <span class="value-text">{{ value }} {{ unit }}</span>
    </div>
    <div class="label">{{ label }}</div>
  </div>
</template>

<script setup>
import { defineProps } from 'vue';

const props = defineProps({
  value: Number,
  max: Number,
  min: Number,
  unit: String,
  label: String,
  type: {
    type: String,
    default: 'default'  // 'temperature', 'moisture', 'gas' 등
  }
});

const normalizedValue = Math.min(Math.max(props.value, props.min), props.max);
const valueRatio = (normalizedValue - props.min) / (props.max - props.min);

const getBarColor = (type, ratio) => {
  if (type === 'moisture') {
    // 낮을수록 위험
    if (ratio < 0.2) return '#f44336';      // 빨강
    else if (ratio < 0.4) return '#ffc107'; // 노랑
    else if (ratio < 0.7) return '#4caf50'; // 초록
    else return '#2196f3';                  // 파랑
  } else if (type === 'pressure' || type === 'humidity') {
    // 중간이 적정
    if (ratio < 0.3 || ratio > 0.9) return '#f44336'; // 위험
    else if (ratio < 0.4 || ratio > 0.8) return '#ffc107';
    else return '#4caf50';
  } else if (type === 'temperature') {
    // 온도: 양쪽 끝이 위험
    if (ratio < 0.2 || ratio > 0.8) return '#f44336'; // 빨강
    else if (ratio < 0.3) return '#ffc107'; // 노랑
    else return '#4caf50'; // 초록
  } else {
    // 기본: 높을수록 위험 (ex. 가스)
    if (ratio < 0.3) return '#2196f3';
    else if (ratio < 0.7) return '#4caf50';
    else if (ratio < 0.9) return '#ffc107';
    else return '#f44336';
  }
};

const barStyle = {
  height: `${valueRatio * 100}%`,
  backgroundColor: getBarColor(props.type, valueRatio)
};
</script>

<style scoped>
.gauge-wrapper {
  display: flex;
  flex-direction: column;
  align-items: center;
  width: 60px;
}

.bar-container {
  position: relative;
  height: 150px;
  width: 63%;
  background: #e0e0e0;
  display: flex;
  align-items: flex-end;
  justify-content: center;
}

.bar {
  width: 100%;
  background-color: #4caf50;
  position: relative; /* ✅ 바 내부에 텍스트 고정 가능하게 함 */
  display: flex;
  justify-content: center;
  align-items: flex-start; /* 상단 배치 */
}

.value-text {
  position: absolute;
  top: -20px;
  left: 50%;                     /* ✅ 가운데 기준 */
  transform: translateX(-50%);   /* ✅ 정확히 중앙 정렬 */
  font-size: 14px;
  color: black;
  white-space: nowrap;           /* ✅ 줄바꿈 방지 */
  min-width: 80px;               /* ✅ 공간 확보 */
  text-align: center;            /* ✅ 글자 중앙 정렬 */
}

.label {
  text-align: center;
  margin-top: 10px;
}
</style>
