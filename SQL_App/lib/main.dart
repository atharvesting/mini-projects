import 'package:flutter/material.dart';
import 'package:sql_practice/data.dart';

void main() {
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  // This widget is the root of your application.
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Flutter Demo',
      theme: ThemeData(
        colorScheme: .fromSeed(seedColor: Colors.deepPurple),
      ),
      home: const MyHomePage(),
      debugShowCheckedModeBanner: false,
    );
  }
}


class MyHomePage extends StatefulWidget {
  const MyHomePage({super.key});

  @override
  State<MyHomePage> createState() => _MyHomePageState();
}

class _MyHomePageState extends State<MyHomePage> {

  List<Entry> list = [];

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text("SQL Test App")
      ),
      body: Column(
        children: [
          Expanded(
            child: Padding(
              padding: const EdgeInsets.all(8.0),
              child: ListView.builder(
                itemCount: list.length,
                itemBuilder: (context, index) {
                  final e = list[index];
                  return Container(
                    height: 50,
                    decoration: BoxDecoration(
                      color: Colors.green[100],
                      borderRadius: BorderRadius.circular(8),
                      border: Border.all()
                    ),
                    child: Padding(
                      padding: const EdgeInsets.symmetric(horizontal: 10), // Adds 10px gap on left and right
                      child: Row(
                        mainAxisAlignment: MainAxisAlignment.spaceBetween, // Pushes children to opposite ends
                        children: [
                          Text(e.title, style: TextStyle(fontWeight: FontWeight.bold)),
                          Text((e.id).toString()),
                        ],
                      ),
                    ),
                  );
                },
                addSemanticIndexes: true,
              ),
            ),
          )
        ],
      ),
      bottomNavigationBar: Padding(
        padding: const EdgeInsets.all(16.0),
        child: Column(
          mainAxisSize: MainAxisSize.min,
          children: [
            Row(
              mainAxisAlignment: MainAxisAlignment.spaceEvenly,
              children: [
                IconButton(onPressed: () {}, icon: Icon(Icons.delete)),
                IconButton(
                  onPressed: () async {
                    final titleController = TextEditingController();
                    await showDialog(
                      context: context, 
                      builder: (context) {
                        return AlertDialog(
                          title: Text("Hello"),
                          content: Column(
                            mainAxisSize: MainAxisSize.min,
                            children: [
                              TextField(
                                controller: titleController,
                                decoration: InputDecoration(
                                  labelText: "Title"
                                ),
                              )
                            ],
                          ),
                          actions: [
                            TextButton(
                              onPressed: () async {
                                if (titleController.text.isNotEmpty) {
                                  final entry = Entry(
                                    id: DateTime.now().millisecondsSinceEpoch,
                                    title: titleController.text,
                                  );

                                  await DBHelper.instance.createEntry(entry);
                                  var result = await DBHelper.instance.fetchAll();
                                  setState(() {
                                    list = result;
                                  });
                                  Navigator.pop(context);
                                }
                              },
                              child: Text("Add")
                            )
                          ],
                        );
                      }
                      
                    );
                  }, 
                  icon: Icon(Icons.add)),
                IconButton(onPressed: () {}, icon: Icon(Icons.update)),
              ],
            ),
            SizedBox(height: 20)
          ],
        ),
      ),
    );
  }
}