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
        stroke="#4caf50"
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
    default: 'default'  // 'temperature', 'moisture', 'gas' 등
  }
});

// 게이지 값 계산
const angle = computed(() => ((props.value - props.min) / (props.max - props.min)) * 180);
const rad = computed(() => (Math.PI / 180) * (180 - angle.value)); // 반시계
const needleX = computed(() => 100 + 90 * Math.cos(rad.value));
const needleY = computed(() => 100 - 90 * Math.sin(rad.value));

// 반원 호의 전체 길이 약 283 (πr)
const dashArray = 283;
const dashOffset = computed(() => dashArray - (angle.value / 180) * dashArray);
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
}
</style>
