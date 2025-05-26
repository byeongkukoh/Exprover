import os
import time
from openai import OpenAI, OpenAIError, RateLimitError

# API 키 로드
def load_env_to_dict(path='.env'):
    env_dict = {}
    try:
        with open(path) as f:
            for line in f:
                line = line.strip()
                if not line or line.startswith('#'):
                    continue
                if '=' not in line:
                    continue
                key, value = line.split('=', 1)
                value = value.strip().strip("'").strip('"')
                env_dict[key.strip()] = value
    except FileNotFoundError:
        print(f"{path} 파일이 없습니다.")
    return env_dict

env = load_env_to_dict(os.path.join(os.path.dirname(__file__), ".env"))

openai_api_key = env.get("OPENAI_API_KEY")

client = OpenAI(api_key=openai_api_key)

def ask_chatbot(prompt: str, retry_count=3, backoff_factor=2) -> str:
    for attempt in range(retry_count):
        try:
            resp = client.chat.completions.create(
                model="gpt-3.5-turbo",
                messages=[{"role": "user", "content": prompt}],
                max_tokens=100,
                temperature=0.7,
                timeout=15
            )
            return resp.choices[0].message.content.strip()

        except RateLimitError as e:
            if attempt < retry_count - 1:
                wait_time = backoff_factor ** attempt
                print(f"요금제 한도를 초과했습니다. {wait_time}초 대기 후 재시도...")
                time.sleep(wait_time)
                continue
            else:
                print("최대 재시도 횟수 초과. 청구 정보를 확인하세요.")
                return ""

        except OpenAIError as e:
            print(f"API 호출 중 오류 발생: {e}")
            return ""

if __name__ == "__main__":
    print("exprover와 대화를 시작합니다! 'exit' 또는 'quit'를 입력하면 종료됩니다.")
    while True:
        # 사용자 입력 받기
        user_input = input("당신: ")
        
        # 종료 조건
        if user_input.lower() in ['exit', 'quit']:
            print("exprover: 대화를 종료합니다. 안녕하세요!")
            break

        # 빈 입력 방지
        if not user_input.strip():
            print("exprover: 질문을 입력해 주세요!")
            continue

        # 챗봇에 질문
        answer = ask_chatbot(user_input)
        if answer:
            print("exprover:", answer)

