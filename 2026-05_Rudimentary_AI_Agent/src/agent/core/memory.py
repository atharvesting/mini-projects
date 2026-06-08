class MemoryModule:
    def __init__(self, sys_prompt: str):
        self.messages: list[dict[str, str]] = [{"role": "system", "content": sys_prompt}]

    def get_messages(self):
        messages = ""
        for mesdic in self.messages:
            messages += mesdic["role"] + ": " + mesdic["content"] + "\n"
        
        return messages