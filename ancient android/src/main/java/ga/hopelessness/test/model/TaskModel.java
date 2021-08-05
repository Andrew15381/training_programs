package ga.hopelessness.test.model;

/*Задание*/
public class TaskModel
{
	/*Ответ, вопрос*/
	private String answer, question;
	/*Баллы за верный ответ*/
	private int points;

	/*Создает новое задание.
	 answer - ответ
	 question - вопрос
	 points - баллы*/
	public TaskModel(String answer, String question, int points)
	{
		this.answer = answer;
		this.question = question;
		this.points = points;
	}

	/*Создает пустое задание.*/
	public TaskModel()
	{
		answer = question = "";
		points = 0;
	}

	/*Возвращает ответ.*/
	public String getAnswer()
	{
		return answer;
	}

	/*Возвращает вопрос.*/
	public String getQuestion()
	{
		return question;
	}

	/*Возвращает баллы.*/
	public int getPoints()
	{
		return points;
	}
}
