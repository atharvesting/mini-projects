from agent.core import runner

def main():
    agint = runner.Agent()
    print(agint.ask_agent("Give me three ways to turn my mood around."))
    print()
    response = agint.ask_agent("What was your second point?")
    print(response)

if __name__ == "__main__":
    main()
