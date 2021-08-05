package ga.hopelessness.test.model;

import com.google.gson.annotations.*;

/*Тест*/
public class TestModel
{
	/*Название, описание, время создания, автор, идентификатор теста*/
	private String name, description, date, author, id;
	/*Темы заданий*/
	@SerializedName("task_themes")
	private String[] taskThemes;

	/*Создает новый тест.
	 name - название
	 description - описание
	 date - дата создания
	 taskThemes - темы заданий
	 id - идентификатор*/
	public TestModel(String name, String description, String date, String author, String[] taskThemes, String id)
	{
		this.name = name;
		this.description = description;
		this.date = date;
		this.author = author;
		this.taskThemes = taskThemes;
		this.id = id;
	}

	/*Возвращает имя.*/
	public String getName()
	{
		return name;
	}

	/*Возвращает описание.*/
	public String getDescription()
	{
		return description;
	}

	/*Возвращает дату создания.*/
	public String getDate()
	{
		return date;
	}

	/*Возвращает автора.*/
	public String getAuthor()
	{
		return author;
	}

	/*Возвращает темы вопросов.*/
	public String[] getTaskThemes()
	{
		return taskThemes;
	}

	/*Возвращает идентификатор.*/
	public String getId()
	{
		return id;
	}
}
