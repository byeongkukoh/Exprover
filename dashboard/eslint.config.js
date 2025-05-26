import js from "@eslint/js";
import globals from "globals";
import pluginVue from "eslint-plugin-vue";

export default [
  js.configs.recommended, // 기본 JavaScript 추천 규칙
  ...pluginVue.configs["flat/essential"], // Vue 3 필수 규칙
  {
    files: ["**/*.{js,mjs,cjs,vue}"], // 대상 파일: JS, Vue
    languageOptions: {
      sourceType: "module",
      globals: {
        ...globals.browser, // 브라우저 환경 글로벌 변수
        defineProps: "readonly", // defineProps 글로벌로 인식
        defineEmits: "readonly", // defineEmits 글로벌로 인식
        defineExpose: "readonly", // defineExpose 글로벌로 인식
      },
    },
    plugins: {
      vue: pluginVue, // Vue 플러그인 명시
    },
    rules: {
      // 필요 시 추가 규칙
      "vue/multi-word-component-names": "off", // 단일 단어 컴포넌트 이름 허용 (선택)
    },
  },
];