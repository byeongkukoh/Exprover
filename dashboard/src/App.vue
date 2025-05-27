<template>
  <div class="layout">
    <div class="chart">
      <div>
        <label style="margin-right: 20px">차량 선택</label>
        <select v-model="selectedCar">
          <option value="EXP">EXP</option>
          <option value="ROVER">ROVER</option>
        </select>
        <button style="margin-left: 20px" @click="viewDetail = !viewDetail">
          {{ viewDetail ? "상세 보기" : "현재 보기" }}
        </button>
      </div>
      <br />
      <div v-if="viewDetail">
        <GaugeGroupVertical
          :temperature="temperature"
          :humidity="humidity"
          :pressure="pressure"
        />
        <GaugeGroupSemi :gas="gas" :moisture="moisture" />
      </div>
      <div v-else>
        <SensorChart :selectedCar="selectedCar" />
      </div>
    </div>

    <div class="ai">
      <ChatBot />
      <RainPredictor />
    </div>
  </div>
  <button v-show="showScrollTop" class="scroll-top-btn" @click="scrollToTop">
    ↑ 맨 위로
  </button>
  <footer class="site-footer">
    <p class="info-line">
      고객센터 <strong>ed0909@naver.com</strong> | 평일 상담시간
      <strong>09:00 ~ 18:00</strong>
    </p>
    <p class="copyright-line">
      본 사이트의 콘텐츠는 저작권법의 보호를 받는 바 무단 전체, 복사, 배포 등을
      금합니다.
    </p>
    <p class="copyright-mark">Copyright © EXPROVER All Rights Reserved.</p>
  </footer>
</template>


<script setup>
import ChatBot from "./components/ChatBot.vue";
import RainPredictor from "./components/RainPredictor.vue";
import { ref, onMounted, watch, onUnmounted } from "vue";
import { fetchSensorData } from "./components/SensorData.js";
import GaugeGroupVertical from "./components/GaugeGroupVertical.vue";
import GaugeGroupSemi from "./components/GaugeGroupSemi.vue";
import SensorChart from "./components/SensorChart.vue";

const showScrollTop = ref(false);

const handleScroll = () => {
  showScrollTop.value = window.scrollY > 300;
};

const scrollToTop = () => {
  window.scrollTo({ top: 0, behavior: "smooth" });
};

onMounted(() => {
  window.addEventListener("scroll", handleScroll);
});

onUnmounted(() => {
  window.removeEventListener("scroll", handleScroll);
});

const selectedCar = ref("EXP");

const temperature = ref(null);
const humidity = ref(null);
const pressure = ref(null);
const gas = ref(null);
const moisture = ref(null);
const viewDetail = ref(true);

const updateSensorValues = async () => {
  const data = await fetchSensorData(selectedCar.value);
  const safe = (v) => (typeof v === "number" && !isNaN(v) ? v : null);

  temperature.value = safe(data.temp);
  humidity.value = safe(data.humi);
  pressure.value = safe(data.pres);
  gas.value = safe(data.gas);
  moisture.value = safe(data.mois);
};

const onClickViewDetail = () => {
  if (viewDetail) viewDetail = false;
  else viewDetail = true;
};
onMounted(() => {
  updateSensorValues();
  setInterval(updateSensorValues, 60 * 60 * 1000); // 60분마다 자동 업데이트
});

// ✅ 차량 선택 변경 감지
watch(selectedCar, () => {
  updateSensorValues();
});
</script>

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
.layout {
  display: flex;
  align-items: flex-start;
  gap: 20px;
  padding: 20px;
}

.chart {
  flex: 2;
  min-width: 0;
}

.ai {
  flex: 1;
  position: sticky;
  top: 20px; /* 상단에서 20px 떨어져 고정 */
  align-self: flex-start;
  height: fit-content;
  display: flex;
  flex-direction: column;
  gap: 20px;
}
.scroll-top-btn {
  position: fixed;
  bottom: 30px;
  right: 30px;
  background-color: #4caf50;
  color: white;
  border: none;
  border-radius: 50px;
  padding: 12px 18px;
  font-size: 16px;
  cursor: pointer;
  box-shadow: 0 2px 8px rgba(0, 0, 0, 0.3);
  z-index: 1000;
  transition: opacity 0.3s ease;
}

.scroll-top-btn:hover {
  background-color: #388e3c;
}
.site-footer {
  background-color: #f5f5f5;
  padding: 15px 20px;
  font-size: 13px;
  color: #666;
  text-align: center;
  line-height: 1.8;
  margin-top: 40px;
}

.site-footer strong {
  color: #2b2b2b;
}

.site-footer .info-line {
  margin-bottom: 4px;
  font-weight: 500;
}

.site-footer .copyright-line {
  margin-bottom: 2px;
  font-size: 12.5px;
}

.site-footer .copyright-mark {
  font-size: 12.5px;
  color: #888;
}
</style>
