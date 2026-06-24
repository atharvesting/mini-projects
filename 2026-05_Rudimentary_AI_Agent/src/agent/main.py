from agent.core import runner
import agent.prompts as pmt

def main():

    agint = runner.Agent(pmt.BRAIN_PROMPT1, pmt.SYS_PROMPT2 + pmt.RESPONSE_FORMAT + pmt.TOOL_GUIDE)

    while True:
        user = input("You > ")
        response = agint.ask_agent(user)  # This triggers a while loop until task is completed
        print("Agint >", response)

if __name__ == "__main__":
    main()
