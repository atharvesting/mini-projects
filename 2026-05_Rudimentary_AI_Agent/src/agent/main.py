from agent.core import runner

def main():
    agint = runner.Agent()
    response = agint.ask_agent("Hello agint, are you alive? do you get the word play done with 'Agint'?")
    print(response)


if __name__ == "__main__":
    main()
