<template>
  <div class="semi-gauge">
    <svg viewBox="0 0 200 100" class="gauge-svg">
      <!-- 회색 배경 반원 -->
      <path
        d="M10 100 A90 90 0 0 1 190 100"
        fill="none"
        stroke="#eee"
        stroke-width="10"
      />

      <!-- 실제 값 채우는 색상 게이지 -->
      <path
        d="M10 100 A90 90 0 0 1 190 100"
        fill="none"
        :stroke="gaugeColor"
        stroke-width="10"
        :stroke-dasharray="dashArray"
        :stroke-dashoffset="dashOffset"
        stroke-linecap="round"
      />

      <!-- 중앙 텍스트 -->
      <text x="100" y="90" text-anchor="middle" font-size="14" font-weight="bold">
        {{ value }} {{ unit }}
      </text>
    </svg>

    <div class="label">{{ label }}</div>
  </div>
</template>

<script setup>
import { computed } from 'vue';

const props = defineProps({
  value: Number,
  max: Number,
  min: Number,
  unit: String,
  label: String,
  type: {
    type: String,
    default: 'default'  // 'gas', 'moisture' 등
  }
});

// 비율 계산
const ratio = computed(() => {
  if (typeof props.value !== 'number' || isNaN(props.value)) return 0;
  const clamped = Math.min(Math.max(props.value, props.min), props.max);
  return (clamped - props.min) / (props.max - props.min);
});

// 게이지 각도 계산
const angle = computed(() => ratio.value * 180);
const dashArray = 283;
const dashOffset = computed(() => dashArray - (angle.value / 180) * dashArray);

// ✅ 색상 결정 함수
const getGaugeColor = (type, r) => {
  if (type === 'moisture') {
    // 낮을수록 위험
    if (r < 0.2) return '#f44336';      // 빨강
    else if (r < 0.4) return '#ffc107'; // 노랑
    else if (r < 0.7) return '#4caf50'; // 초록
    else return '#2196f3';              // 파랑
  } else if (type === 'gas') {
    // 높을수록 위험
    if (r < 0.3) return '#2196f3';      // 파랑
    else if (r < 0.7) return '#4caf50'; // 초록
    else if (r < 0.9) return '#ffc107'; // 노랑
    else return '#f44336';             // 빨강
  } else {
    // 기본
    return '#4caf50';
  }
};

const gaugeColor = computed(() => getGaugeColor(props.type, ratio.value));
</script>

<style scoped>
.semi-gauge {
  display: flex;
  flex-direction: column;
  align-items: center;
  width: 200px;
  margin: 20px;
}

.gauge-svg {
  width: 100%;
  height: auto;
}

.label {
  font-size: 14px;
  color: #444;
  margin-top: 8px;
  font-weight: bold;
}
</style>
