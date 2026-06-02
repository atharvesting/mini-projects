class CalculatorTool():
    
    def get_schema(self):
        return {
            "name": "calculators",
            "descriptions": "Performs basic mathematical calculations",
            "input_schema": {
                "type": "object",
                "properties": {
                    "expression": {
                        "type": "string",
                        "description": "Mathematical expression to evaluate"
                    }
                },
                "required": ["expression"]
            }
        }
        
    def execute(self, expression):
        try:
            result = eval(expression)
            return {"result": result}
        except:  # noqa: E722
            return {"error": "Invalid mathematical expression"}