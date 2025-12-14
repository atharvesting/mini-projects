import 'package:sqflite/sqflite.dart';
import 'package:path/path.dart';

// final path = join(getDatabasesPath(), 'test.db');

class Entry {

  final int? id;
  final String title;

  const Entry({
    required this.id, 
    required this.title
  });

  Map<String, dynamic> toMap() {
    return {
      'id': id,
      'title': title
    };
  }

  factory Entry.fromMap(Map<String, dynamic> map) {
    return Entry(
      id: map['id'],
      title: map['title']
    );
  }
}

class DBHelper {

  static final DBHelper instance = DBHelper._init();
  DBHelper._init();

  static Database? _db;

  Future<Database> get db async {
    // If we have the connection to the database:
    if (_db != null) return _db!;

    // If we don't:
    _db = await _initDB('test.db');
    return _db!;
  }

  Future<Database> _initDB(String filePath) async {
    final dbPath = await getDatabasesPath();
    final path = join(dbPath, filePath);
    return await openDatabase(path, 
      version: 1, 
      onCreate: _createDB
    );
  }

  Future<void> _createDB(Database db, int version) async {
    await db.execute('''
      CREATE TABLE entries (
      id INTEGER PRIMARY KEY AUTOINCREMENT,
      title TEXT NOT NULL
      )
    ''');
  }

  Future<int> createEntry(Entry entry) async {
    final db = await instance.db;

    return await db.insert(
      'entries',
      entry.toMap(),
      conflictAlgorithm: ConflictAlgorithm.replace,
    );
  }

  Future<List<Entry>> fetchAll() async {
    final db = await instance.db;
    final maps = await db.query('entries');

    return maps.map((m) => Entry.fromMap(m)).toList();
  }

  Future<int> deleteEntry(int id) async {
    final db = await instance.db;

    return await db.delete(
      'entries',
      where: 'id = ?',
      whereArgs: [id], 
    );
  }

  Future<int> clearAll() async {
    final db = await instance.db;

    return db.delete('entries');
  }
}
