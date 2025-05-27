module.exports = {
    root: true,
    env: {
      node: true,
    },
    extends: [
      'plugin:vue/vue3-essential',
      'eslint:recommended'
    ],
    // parser 제거 또는 기본 파서 사용
    parserOptions: {
      ecmaVersion: 2020,
      sourceType: 'module'
    },
    rules: {
      'no-console': 'off',
      'no-debugger': 'off'
    }
  }