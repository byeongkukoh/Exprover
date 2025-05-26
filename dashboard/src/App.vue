<script setup>
import ChatBot from "./components/ChatBot.vue"
import RainPredictor from "./components/RainPredictor.vue"
</script>

<template>
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
