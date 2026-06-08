from agent.core import runner
from agent.llm import client
from agent.prompts import SYS_PROMPT2, TOOL_GUIDE
from agent.tools.calculator import Calculator

def main():

    agint = runner.Agent(SYS_PROMPT2 + TOOL_GUIDE)

    while True:
        user = input("You > ")
        answer = agint.ask_agent(user)
        print(f"{"Thinking":=^20}\n" + answer[1] + "\n")
        print("Agint >", answer[0])

if __name__ == "__main__":
    main()
