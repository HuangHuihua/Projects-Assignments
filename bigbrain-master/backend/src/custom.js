
// import { v4 as uuidv4 } from 'uuid';

/*
 For a given data structure of a question, produce another
 object that doesn't contain any important meta data (e.g. the answer)
 to return to a "player"
*/

export const quizQuestionPublicReturn = question => {
  const newQuestion = {
  id: question.id,
  title: question.title,
  photo: question.photo,
  StartAt: new Date().getTime(),
  Type: question.Type,
  TimeLimit: question.TimeLimit,
  point: question.point,
  choice: [
    {
      choiceId: question.choice[0].choiceId,
      choiceContent: choiceA,
    },
    {
      choiceId: question.choice[1].choiceId,
      choiceContent: choiceB,
    },
    {
      choiceId: question.choice[2].choiceId,
      choiceContent: choiceC,
    },
    {
      choiceId: question.choice[3].choiceId,
      choiceContent: choiceD,
    },
    {}
  ]
};
  console.log('See question: ', question);
  return newQuestion;
};

/*
 For a given data structure of a question, get the IDs of
 the correct answers (minimum 1).
*/
export const quizQuestionGetCorrectAnswers = question => {
  let correctAnswers = [];
  for(let i  = 0; i < question.length; i++ ){
    if(choice[i].IsAnswer == true){
      correctAnswers = [...correctAnswers, choice[i].choiceId];
      return correctAnswers;
    }
  }
};

/*
For a given data structure of a question, get the IDs of
all of the answers, correct or incorrect.
*/
export const quizQuestionGetAnswers = question => {
  const { choice = [] } = question;
  return choice.map(id => id.choiceId);
  // return [
  //   123,
  //   456,
  //   678,
  // ]; // For a single answer
};

/*
 For a given data structure of a question, get the duration
 of the question once it starts. (Seconds)
*/
export const quizQuestionGetDuration = question => {
  return new Date().getTime()-question.StartAt;
};
