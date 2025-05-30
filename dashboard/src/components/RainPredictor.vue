<template>
    <div class="rain-predictor">
        <button @click="getPrediction" :disabled="loading">
            <span v-if="!loading">🌧️ 강수 예측</span>
            <span v-else>⏳ 예측 중...</span>
        </button>
        <div v-if="error" class="error">{{ error }}</div>
        <div v-if="result" class="result"
            :class="{ 'rain': result.result === 'Rain', 'no-rain': result.result === 'No Rain' }">
            <p>
                예측 결과:
                <b>
                    <span v-if="result.result === 'Rain'">🌧️</span>
                    <span v-else>☀️</span>
                    {{ result.result }}
                </b>
            </p>
            <p>확률: {{ (result.prob * 100).toFixed(1) }}%</p>
        </div>
    </div>
</template>

<script setup>
import { ref } from 'vue'
import axios from 'axios'

const result = ref(null)
const loading = ref(false)
const error = ref('')

const BACKEND_URL = 'api/predict' // 라즈베리파이 IP로 변경!

async function getPrediction() {
    result.value = null
    error.value = ''
    loading.value = true
    try {
        const res = await axios.get(BACKEND_URL)
        result.value = res.data
    } catch (e) {
        error.value = e?.response?.data?.message || e.message || '예측 요청 실패'
    } finally {
        loading.value = false
    }
}
</script>

<style scoped>
.rain-predictor {
    background: linear-gradient(135deg, #f7fafc, #d7e9fa 80%);
    margin: 24px auto;
    padding: 32px 28px 24px;
    border-radius: 18px;
    box-shadow: 0 6px 24px rgba(42, 86, 155, 0.09);
    width: 450px;
    text-align: center;
}

button {
    padding: 12px 32px;
    font-size: 1.18em;
    margin-bottom: 20px;
    border-radius: 24px;
    border: none;
    background: linear-gradient(90deg, #457efa 50%, #4fd1c5 100%);
    color: white;
    font-weight: bold;
    cursor: pointer;
    box-shadow: 0 2px 8px rgba(70, 130, 180, 0.1);
    transition: 0.2s;
}

button:disabled {
    opacity: 0.5;
    cursor: not-allowed;
}

button:hover:enabled {
    filter: brightness(1.07);
}

.result {
    margin-top: 16px;
    font-size: 1.18em;
    font-weight: 600;
    transition: background 0.3s;
    padding: 16px;
    border-radius: 12px;
}

.result.rain {
    background: #dbeafe;
    color: #2563eb;
}

.result.no-rain {
    background: #f3f4f6;
    color: #737373;
}

.error {
    color: #d32f2f;
    margin-top: 10px;
}
</style>
